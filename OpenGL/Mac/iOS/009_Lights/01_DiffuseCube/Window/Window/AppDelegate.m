//
//  AppDelegate.m
//  Window
//
//  Created by user172418 on 7/3/21.
//  Copyright © 2021 com.student. All rights reserved.
//

#import "AppDelegate.h"
#import "ViewController.h"
#import "GLESView.h"

@implementation AppDelegate
{
    @private
    UIWindow *window;
    ViewController *viewController;
    GLESView *view;
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    CGRect win_rect = [[UIScreen mainScreen]bounds];
    
    window = [[UIWindow alloc]initWithFrame:win_rect];
    // create view controller
    viewController = [[ViewController alloc]init];
    
    [window setRootViewController:viewController];
    view = [[GLESView alloc]initWithFrame:win_rect];
    [viewController setView:view];
    [window makeKeyAndVisible];
    [view startAnimation];
    // addref by init and setController thats why released one
    // if we dont do it, its automatically done by ARC
    [view release];
    return YES;
}

-(void)applicationWillResignActive:(UIApplication *)application
{
    // code
    [view stopAnimation];
    
}

-(void)applicationDidEnterBackground:(UIApplication *)application
{
    // code
}

-(void)applicationWillEnterForeground:(UIApplication *)application
{
    // code
}

-(void)applicationDidBecomeActive:(UIApplication *)application
{
    // code
    [view startAnimation];
}

-(void)applicationWillTerminate:(UIApplication *)application
{
    // coode
    [view stopAnimation];
    
}

-(void)dealloc
{
    [view release];
    [viewController release];
    [window release];
    [super dealloc];
}

@end
