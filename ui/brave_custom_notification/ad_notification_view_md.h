/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef UI_BRAVE_CUSTOM_NOTIFICATION_VIEWS_NOTIFICATION_VIEW_MD_H_
#define UI_BRAVE_CUSTOM_NOTIFICATION_VIEWS_NOTIFICATION_VIEW_MD_H_

#include <vector>

#include "chrome/browser/ui/browser_list_observer.h"
#include "base/gtest_prod_util.h"
#include "base/macros.h"
#include "base/optional.h"
#include "base/time/time.h"
#include "brave/ui/brave_custom_notification/notification_view.h"
#include "ui/views/animation/ink_drop_observer.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/button/label_button.h"
#include "ui/views/controls/textfield/textfield_controller.h"

namespace views {
class ImageButton;
class Label;
class LabelButton;
class ProgressBar;
class RadioButton;
class Textfield;
}

namespace brave_custom_notification {

class NotificationHeaderView;
class ProportionalImageView;

class LargeImageView : public views::View {
 public:
  LargeImageView();
  ~LargeImageView() override;

  void SetImage(const gfx::ImageSkia& image);

  void OnPaint(gfx::Canvas* canvas) override;
  const char* GetClassName() const override;

 private:
  gfx::Size GetResizedImageSize();

  gfx::ImageSkia image_;

  DISALLOW_COPY_AND_ASSIGN(LargeImageView);
};

// View that displays all current types of notification (web, basic, image, and
// list) except the custom notification. Future notification types may be
// handled by other classes, in which case instances of those classes would be
// returned by the Create() factory method below.
class AdNotificationViewMD
    : public NotificationView,
      public views::InkDropObserver,
      public views::ButtonListener {
 public:
  explicit AdNotificationViewMD(const Notification& notification);
  ~AdNotificationViewMD() override;

  void Activate();

  void AddBackgroundAnimation(const ui::Event& event);
  void RemoveBackgroundAnimation();

  // NotificationView:
  void AddLayerBeneathView(ui::Layer* layer) override;
  void RemoveLayerBeneathView(ui::Layer* layer) override;
  void Layout() override;
  void OnFocus() override;
  bool OnMousePressed(const ui::MouseEvent& event) override;
  bool OnMouseDragged(const ui::MouseEvent& event) override;
  void OnMouseReleased(const ui::MouseEvent& event) override;
  void OnMouseEvent(ui::MouseEvent* event) override;
  void OnGestureEvent(ui::GestureEvent* event) override;
  void PreferredSizeChanged() override;
  std::unique_ptr<views::InkDrop> CreateInkDrop() override;
  std::unique_ptr<views::InkDropRipple> CreateInkDropRipple() const override;
  std::unique_ptr<views::InkDropMask> CreateInkDropMask() const override;
  SkColor GetInkDropBaseColor() const override;
  void UpdateWithNotification(const Notification& notification) override;
  void ButtonPressed(views::Button* sender, const ui::Event& event) override;
  void UpdateCornerRadius(int top_radius, int bottom_radius) override;
  NotificationControlButtonsView* GetControlButtonsView() const override;

  void InkDropAnimationStarted() override;
  void InkDropRippleAnimationEnded(views::InkDropState ink_drop_state) override;

 private:
  class AdNotificationViewMDPathGenerator;

  void UpdateControlButtonsVisibilityWithNotification(
      const Notification& notification);

  void CreateOrUpdateViews(const Notification& notification);

  void CreateOrUpdateContextTitleView(const Notification& notification);
  void CreateOrUpdateTitleView(const Notification& notification);
  void CreateOrUpdateNotificationView(const Notification& notification);
  void CreateOrUpdateProgressBarView(const Notification& notification);
  void CreateOrUpdateProgressStatusView(const Notification& notification);
  void CreateOrUpdateListItemViews(const Notification& notification);
  void CreateOrUpdateIconView(const Notification& notification);
  void CreateOrUpdateSmallIconView(const Notification& notification);
  void CreateOrUpdateImageView(const Notification& notification);
  void CreateOrUpdateActionButtonViews(const Notification& notification);

  bool IsExpandable();
  void ToggleExpanded();
  void UpdateViewForExpandedState(bool expanded);
  void ToggleInlineSettings(const ui::Event& event);

  // Returns the list of children which need to have their layers created or
  // destroyed when the ink drop is visible.
  std::vector<views::View*> GetChildrenForLayerAdjustment() const;

  views::InkDropContainerView* const ink_drop_container_;

  // View containing close and settings buttons
  std::unique_ptr<NotificationControlButtonsView> control_buttons_view_;

  // Whether this notification is expanded or not.
  bool expanded_ = true;

  // True if the notification is expanded/collapsed by user interaction.
  // If true, MessagePopupCollection will not auto-collapse the notification.
  bool manually_expanded_or_collapsed_ = true;

  // Whether hiding icon on the right side when expanded.
  bool hide_icon_on_expanded_ = false;

  // Number of total list items in the given Notification class.
  int list_items_count_ = 0;

  // Describes whether the view should display a hand pointer or not.
  bool clickable_;

  // Container views directly attached to this view.
  NotificationHeaderView* header_row_ = nullptr;
  views::View* content_row_ = nullptr;
  views::View* actions_row_ = nullptr;
  views::View* settings_row_ = nullptr;

  // Containers for left and right side on |content_row_|
  views::View* left_content_ = nullptr;
  views::View* right_content_ = nullptr;

  // Views which are dynamically created inside view hierarchy.
  views::Label* title_view_ = nullptr;
  views::Label* message_view_ = nullptr;
  views::Label* status_view_ = nullptr;
  views::View* image_container_view_ = nullptr;
  std::vector<views::View*> item_views_;
  views::ProgressBar* progress_bar_view_ = nullptr;
  views::View* action_buttons_row_ = nullptr;

  // Counter for view layouting, which is used during the CreateOrUpdate*
  // phases to keep track of the view ordering. See crbug.com/901045
  int left_content_count_;

  // Views for inline settings.
  views::RadioButton* block_all_button_ = nullptr;
  views::RadioButton* dont_block_button_ = nullptr;
  views::LabelButton* settings_done_button_ = nullptr;

  // Owned by views properties. Guaranteed to be not null for the lifetime of
  // |this| because views properties are the last thing cleaned up.
  AdNotificationViewMDPathGenerator* highlight_path_generator_ = nullptr;

  std::unique_ptr<ui::EventHandler> click_activator_;

  base::TimeTicks last_mouse_pressed_timestamp_;

  base::WeakPtrFactory<AdNotificationViewMD> weak_ptr_factory_{this};

  DISALLOW_COPY_AND_ASSIGN(AdNotificationViewMD);
};

}

#endif