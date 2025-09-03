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

	void updateColor(GJComment* comm) {
		auto color = user_data::get<ccColor3B>(comm, "camila314.comment-color");

		if (color.isOk()) {
			if (auto node = static_cast<TextArea*>(this->getChildByIDRecursive("comment-text-area"))) {
				node->colorAllLabels(color.unwrap());
			}

			if (auto node = static_cast<CCLabelBMFont*>(this->getChildByIDRecursive("comment-text-label"))) {
				node->setColor(color.unwrap());
			}

			if (auto node = typeinfo_cast<CCRGBAProtocol*>(this->getChildByIDRecursive("prevter.comment_emojis/comment-text-area"))) {
				node->setColor(color.unwrap());
			}

			if (auto node = typeinfo_cast<CCRGBAProtocol*>(this->getChildByIDRecursive("prevter.comment_emojis/comment-text-label"))) {
				node->setColor(color.unwrap());
			}
		}
	}

	void loadFromComment(GJComment* comm) {
		CommentCell::loadFromComment(comm);

		if (user_data::contains(comm, "camila314.comment-color")) {
			updateColor(comm);
		} else {
			this->addEventListener<user_data::CommentFilter>([this](GJComment* comm) {
				updateColor(comm);
			}, comm->m_accountID);
		}
	}
};


$on_mod(Loaded) {
	auto col = Mod::get()->getSettingValue<ccColor3B>("comment-color");
	user_data::upload(col, "camila314.comment-color");

	listenForSettingChangesV3<ccColor3B>("comment-color", [](ccColor3B col) {
		user_data::upload(col, "camila314.comment-color");
	});

}