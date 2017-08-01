//
//  LoginAndShareHeader.h
//  LoginAndShare
//
//  Created by 一公里 on 16/10/14.
//  Copyright © 2016年 第三方登录与分享. All rights reserved.
//

#ifndef LoginAndShareConfig_h
#define LoginAndShareConfig_h
typedef enum {
    kQQ = 1,   //QQ分享与登录
    kSina = 2,//新浪微博分享与登录
    kWeChat = 3,  //微信登录
    
    //注:以下只可用于分享
    kQQZone = 4,   //QQ空间分享
    kWeChatFriend = 5,   //微信好友分享
    kWeChatFriendCircle = 6, //微信朋友圈
    kWeChatCollect = 7, //微信收藏
    kSinaFriend = 8
    
} kLoginShareType;


typedef enum {
    kUserCancel = 1,   //用户取消
    kErro = 2,   //其他错误
} kFailType;

//#ifndef __OPTIMIZE__
//#define NSLog(...) NSLog(__VA_ARGS__)
//#else
//#define NSLog(...){}
//#endif

/**
 QQ第三方登录与分享配置信息
 */
#import <TencentOpenAPI/TencentOAuth.h>
#import <TencentOpenAPI/QQApiInterface.h>

#define QQ_APP_ID @"1105647487"
#define QQ_APP_ID_16 @"41E6D77F"

/**
 新浪微博第三方登录与分享配置信息
 */
#import "WeiboSDK.h"

#define SINA_APP_ID @"2145619015"
#define SINA_BACK_URL @"http://sns.whalecloud.com/sina2/callback"
#define WeiboDidLoginNotification @"weiboDidLoginNotification"
#define WeiboDidShareNotification @"weiboDidShareNotification"

/**
 第三方登录与分享配置信息
 */
#import "WXApi.h"
#define WECHAT_APP_ID @"wx215893eaf319445f"
#define WECHAT_APP_SECRET @"7c09e3d173d343a36c11acbed6644c63"
#define WeChatDidLoginNotification @"weChatDidLoginNotification"
#define WeChatDidShareNotification @"weChatDidShareNotification"

#endif /* LoginAndShareHeader_h */
