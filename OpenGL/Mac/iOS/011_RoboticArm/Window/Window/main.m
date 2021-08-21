//
//  main.m
//  Window
//
//  Created by user172418 on 7/3/21.
//  Copyright © 2021 com.student. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"

int main(int argc, char * argv[])
{
    NSString *appDelegateClassName;
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc]init];

    appDelegateClassName = NSStringFromClass([AppDelegate class]);
    int ret = UIApplicationMain(argc, argv, nil, appDelegateClassName);
    
    [pool release];
    return(ret);
}
