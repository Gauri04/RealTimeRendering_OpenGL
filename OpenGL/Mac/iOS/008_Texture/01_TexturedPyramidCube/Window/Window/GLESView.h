//
//  MyView.h
//  Window
//
//  Created by user172418 on 7/4/21.
//  Copyright © 2021 com.student. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface GLESView : UIView<UIGestureRecognizerDelegate>
-(void) startAnimation;
-(void) stopAnimation;
-(void) initialize;
-(void) updateFn;
-(GLuint)loadTextureFromBMP:(NSString *)imageFileName :(NSString *)extension;
@end

