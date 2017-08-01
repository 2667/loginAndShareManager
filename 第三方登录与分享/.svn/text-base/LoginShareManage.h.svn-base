//
//  LoginShareManage.h
//  LoginAndShare
//
//  Created by 一公里 on 16/10/14.
//  Copyright © 2016年 第三方登录与分享. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LoginAndShareConfig.h"
@interface LoginShareManage : NSObject
@property (nonatomic, strong) TencentOAuth *tencentOAuth;
@property (nonatomic, strong) NSArray *permissions;

+ (id)shareLoginShareManage;
+ (BOOL)HandleOpenURL:(NSURL *)url;
/*
 QQ AppId设置
 **/
+ (void)TencentWithDelegate:(id<TencentSessionDelegate>)delegate;
/**
 新浪微博 AppId设置
 */
+ (void)SinaWeibo;
/**
 微信 AppId设置
 */
+ (void)WeChat;


@end
