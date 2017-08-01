//
//  ShareManage.h
//  LoginAndShare
//
//  Created by 一公里 on 16/10/14.
//  Copyright © 2016年 第三方登录与分享. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LoginAndShareConfig.h"

typedef void(^ShareSuccessBlock)();
typedef void(^ShareFailBlock)(kFailType failType);

@interface ShareManage : NSObject<QQApiInterfaceDelegate>
+ (instancetype)shareShareManager;
@property (nonatomic, copy) ShareSuccessBlock successBlock;
@property (nonatomic, copy) ShareFailBlock failBlock;
/**
 第三方分享
 @param url 视频内容的目标URL
 @param title 分享内容的标题
 @param description 分享内容的描述
 @param imageUrl 分享内容的预览图像
 @param successBlock 分享成功返回
 @param failBlock 分享失败返回
 */
+ (void)shareType:(kLoginShareType)type url:(NSString *)url title:(NSString *)title  description:(NSString *)description imageUrl:(NSString *)imageUrl successBlock:(ShareSuccessBlock)successBlock failBlock:(ShareFailBlock)failBlock;

@end
