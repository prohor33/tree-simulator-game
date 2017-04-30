#pragma once

USING_NS_CC;

namespace visu_utils {
    
// Грузит спрайт размером с чать от экрана, w = (0.0f, 1.0f], позиция центра также относительная
cocos2d::Sprite* LoadSpriteByHeight(float h, const char* file, cocos2d::Vec2 p = cocos2d::Vec2(0.5f, 0.5f));
cocos2d::Sprite* LoadSpriteByWidth(float w, const char* file, cocos2d::Vec2 p = cocos2d::Vec2(0.5f, 0.5f));
void ToBottom(cocos2d::Node* s, float shift_from_bottom = 0.0f, bool pxl = false);
void ToTop(cocos2d::Node* s, float shift_from_top = 0.0f, bool pxl = false);
void ToLeft(cocos2d::Node* s, float shift_from_left = 0.0f, bool pxl = false);
void ToRight(cocos2d::Node* s, float shift_from_right = 0.0f, bool pxl = false);
void ChangeWidth(cocos2d::Node* s, float w);
void ChangeHeight(cocos2d::Node* s, float h);
void ChangeSizeByWidth(cocos2d::Node* s, float w);
void ChangeSizeByHeight(cocos2d::Node* s, float h);
cocos2d::Sprite* LoadSpriteFullScreen(const char* file, float target_s = 1.f);
void MoveX(cocos2d::Node* s, float x);
void MoveY(cocos2d::Node* s, float y);
void MoveXPxl(cocos2d::Node* s, float x);
void MoveYPxl(cocos2d::Node* s, float y);
void TransferToParent(cocos2d::Node* old_parent, cocos2d::Node* new_parent, cocos2d::Node* target);
cocos2d::Vec2 GetPositionInParent(cocos2d::Node* old_parent, cocos2d::Node* new_parent, cocos2d::Vec2 p);
cocos2d::Vec2 GetPositionInParent(cocos2d::Node* old_parent, cocos2d::Vec2 new_parent_world_p, cocos2d::Vec2 p);
cocos2d::Vec2 GetPositionInParent(cocos2d::Vec2 old_parent_world_p, cocos2d::Node* new_parent, cocos2d::Vec2 p);
void RepeatSpriteToWidth(cocos2d::Sprite* s, float width);
void RepeatSpriteToHeight(cocos2d::Sprite* s, float height);
void MoveToXCenter(cocos2d::Node* s, float x);
void MoveToYCenter(cocos2d::Node* s, float y);
void MoveToCenter(cocos2d::Node* s, float x, float y);
void AddDebugPositionLogger(cocos2d::Sprite* s);
void AddOnClickListener(cocos2d::Node* s, OnClickF on_click, double on_touch_coef = 1.f, OnClickLocationF on_click_location = nullptr);
void AddOnClickListener(cocos2d::ui::Button* s, OnClickButtonF on_click, double on_touch_coef = 1.f);
void AddOnMoveListener(Node* s, OnMoveF on_move, double on_touch_coef);
float GetScreenRatio();
void PositionInParent(cocos2d::Node* n, cocos2d::Node* parent, cocos2d::Vec2 p);
void AdditionalScale(cocos2d::Node* s, float scale);

}   // visu_utils
