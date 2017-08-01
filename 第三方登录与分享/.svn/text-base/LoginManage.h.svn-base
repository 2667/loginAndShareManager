//
//  LoginManage.h
//  LoginAndShare
//
//  Created by 一公里 on 16/10/14.
//  Copyright © 2016年 第三方登录与分享. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LoginAndShareConfig.h"
typedef void(^LoginFail)(kFailType faileType);

//注:code只在微信授权中使用
typedef void(^UserInfo)(NSString *userName,NSString *userHeadImage,NSString *sex,NSString *openId,NSString *accessToken,NSString *code);

@interface LoginManage : NSObject
+ (instancetype)shareLoginManage;
/**
 第三方授权登录授权登录
 @param type 传入登录方式 (QQ 新浪 微信)
 @param userInfo 登录成功后用户的基本信息
 */
+ (void)login:(kLoginShareType)type userInfo:(UserInfo)userInfo loginFail:(LoginFail)fail;
@end
