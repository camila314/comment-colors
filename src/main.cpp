#include <Geode/Geode.hpp>
#include <hiimjasmine00.user_data_api/include/UserDataAPI.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/async.hpp>
#include <chrono>

using namespace geode::prelude;
using namespace geode::event::v2;

#include <Geode/modify/CommentCell.hpp>
class $modify(CommentCell) {
	void loadFromComment(GJComment* comm) {
		int id = comm->m_accountID;
		std::string url = std::string("https://playerdata.hiimjasmine00.com/v2/") + std::to_string(id);
		web::WebRequest().timeout(std::chrono::seconds(3)).get(url).listen([id, this](auto out) {
			auto color = out->json()
				.andThen([id](matjson::Value k) { return k.get(std::to_string(id)).copied(); })
				.andThen([](auto k) { return k.get("comment-color").copied(); })
				.andThen([](auto k) { return k.template as<ccColor3B>(); });

			if (color.isOk()) {
				if (auto node = this->getChildByIDRecursive("comment-text-area")) {
					reinterpret_cast<TextArea*>(node)->colorAllLabels(color.unwrap());
				}
			}
		});


		this->CommentCell::loadFromComment(comm);
	}
};


$on_mod(Loaded) {
	new EventListener(+[](std::shared_ptr<SettingV3> thing) {
		log::info("CHANGED COLOR");
		auto col = Mod::get()->getSettingValue<ccColor3B>("comment-color");
		user_data::upload(col, "comment-color");
	}, SettingChangedFilterV3(Mod::get(), "comment-color"));

}