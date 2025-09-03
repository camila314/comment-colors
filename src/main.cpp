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
		CommentCell::loadFromComment(comm);

		int id = comm->m_accountID;
		std::string url = std::string("https://playerdata.hiimjasmine00.com/v2/") + std::to_string(id);

		this->retain();
		web::WebRequest().timeout(std::chrono::seconds(3)).get(url).listen([id, this](auto out) {
			auto color = out->json()
				.andThen([id](matjson::Value k) { return k.get(std::to_string(id)).copied(); })
				.andThen([](auto k) { return k.get("camila314.comment-color").copied(); })
				.andThen([](auto k) { return k.template as<ccColor3B>(); });

			if (color.isOk()) {
				if (auto node = this->getChildByIDRecursive("comment-text-area")) {
					reinterpret_cast<TextArea*>(node)->colorAllLabels(color.unwrap());
				}

				if (auto node = this->getChildByIDRecursive("comment-text-label")) {
					reinterpret_cast<CCLabelBMFont*>(node)->setColor(color.unwrap());
				}

				if (auto node = this->getChildByIDRecursive("prevter.comment_emojis/comment-text-area")) {
					reinterpret_cast<CCRGBAProtocol*>(node)->setColor(color.unwrap());
				}
			}

			this->release();
		});
	}
};


$on_mod(Loaded) {
	auto col = Mod::get()->getSettingValue<ccColor3B>("comment-color");
	user_data::upload(col, "camila314.comment-color");

	new EventListener(+[](std::shared_ptr<SettingV3> thing) {
		auto col = Mod::get()->getSettingValue<ccColor3B>("comment-color");
		user_data::upload(col, "camila314.comment-color");
	}, SettingChangedFilterV3(Mod::get(), "comment-color"));

}