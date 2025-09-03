#include <Geode/Geode.hpp>
#include <hiimjasmine00.user_data_api/include/events/Comment.hpp>
#include <hiimjasmine00.user_data_api/include/UserDataAPI.hpp>

using namespace geode::prelude;
using namespace geode::event::v2;

#include <Geode/modify/CommentCell.hpp>
class $modify(CommentCell) {

	static void onModify(auto& self) {
		if (!self.setHookPriorityAfterPost("CommentCell::loadFromComment", "prevter.comment_emojis")) {
		    geode::log::warn("Failed to set hook priority.");
		}
	}

	void loadFromComment(GJComment* comm) {
		CommentCell::loadFromComment(comm);

		this->addEventListener<user_data::CommentFilter>([this](GJComment* comm) {
			auto color = user_data::get<ccColor3B>(comm, "camila314.comment-color");

			if (color.isOk()) {
				if (auto node = this->getChildByIDRecursive("comment-text-area")) {
					reinterpret_cast<TextArea*>(node)->colorAllLabels(color.unwrap());
				}

				if (auto node = this->getChildByIDRecursive("comment-text-label")) {
					reinterpret_cast<CCLabelBMFont*>(node)->setColor(color.unwrap());
				}

				if (auto node = typeinfo_cast<CCRGBAProtocol*>(this->getChildByIDRecursive("prevter.comment_emojis/comment-text-area"))) {
					log::info("yeah!!!");
					node->setColor(color.unwrap());
				}

				if (auto node = typeinfo_cast<CCRGBAProtocol*>(this->getChildByIDRecursive("prevter.comment_emojis/comment-text-label"))) {
					log::info("yeah!!!");
					node->setColor(color.unwrap());
				}
			}
		}, comm->m_accountID);
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