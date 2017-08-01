//
//  LoginShareManage.m
//  LoginAndShare
//
//  Created by 一公里 on 16/10/14.
//  Copyright © 2016年 第三方登录与分享. All rights reserved.
//

#import "LoginShareManage.h"
#import "ShareManage.h"
#import "LoginManage.h"
//#import "libWeiboSDK/Aspects.h"
static LoginShareManage *_loginShareManager = nil;

@interface LoginShareManage ()<WXApiDelegate>

@end
@implementation LoginShareManage

+ (instancetype)shareLoginShareManage{
    static dispatch_once_t predicate; dispatch_once(&predicate, ^{
        _loginShareManager = [[LoginShareManage alloc]init];
    });
    return _loginShareManager;
}
/*
 QQ AppId设置
 **/
+ (void)TencentWithDelegate:(id<TencentSessionDelegate>)delegate{
    LoginShareManage *manager = [LoginShareManage shareLoginShareManage];
    manager.tencentOAuth = [[TencentOAuth alloc]initWithAppId:QQ_APP_ID andDelegate:delegate];
    manager.permissions = [NSArray arrayWithObjects:kOPEN_PERMISSION_GET_USER_INFO,kOPEN_PERMISSION_GET_SIMPLE_USER_INFO, kOPEN_PERMISSION_ADD_SHARE,nil];
}
/**
 新浪微博 AppId设置
 */
+ (void)SinaWeibo{
    static dispatch_once_t predicate; dispatch_once(&predicate, ^{
        [WeiboSDK enableDebugMode:NO];
        [WeiboSDK registerApp:SINA_APP_ID];
    });
}
/**
 微信 AppId设置
 */
+ (void)WeChat{
    static dispatch_once_t predicate; dispatch_once(&predicate, ^{
//        [WXApi registerApp:WECHAT_APP_ID withDescription:@"WeChat"];
        [WXApi registerApp:WECHAT_APP_ID];
    });
}
/**
 处理第三方软件唤起的跳转请求
 @param url 待处理的url跳转请求
 //param delegate 第三方应用用于处理来至第三方软件请求及响应的委托对象
 @return 跳转请求处理结果，YES表示成功处理，NO表示不支持的请求协议或处理失败
 */
+ (BOOL)HandleOpenURL:(NSURL *)url{
    
    DLog(@"%@",url.absoluteString);
    if ([url.absoluteString hasPrefix:[NSString stringWithFormat:@"QQ%@",QQ_APP_ID_16]] || [url.absoluteString hasPrefix:[NSString stringWithFormat:@"tencent%@",QQ_APP_ID]]) {
        [QQApiInterface handleOpenURL:url delegate:[ShareManage shareShareManager]];
        return [TencentOAuth HandleOpenURL:url];
    }else if ([url.absoluteString hasPrefix:[NSString stringWithFormat:@"wb%@",SINA_APP_ID]]) {
        return [WeiboSDK handleOpenURL:url delegate:[LoginShareManage shareLoginShareManage]];
    }else if ([url.absoluteString hasPrefix:[NSString stringWithFormat:@"%@",WECHAT_APP_ID]]) {
        return [WXApi handleOpenURL:url delegate:[LoginShareManage shareLoginShareManage]];
    }
    return YES;
}

/**
 收到一个来自微博客户端程序的响应
 
 收到微博的响应后，第三方应用可以通过响应类型、响应的数据和 WBBaseResponse.userInfo 中的数据完成自己的功能
 @param response 具体的响应对象
 */
- (void)didReceiveWeiboResponse:(WBBaseResponse *)response{
    
    if ([response isKindOfClass:WBAuthorizeResponse.class])//微博登录的回调
    {
        
        NSString *userId = [(WBAuthorizeResponse *)response userID];
        NSString *accessToken = [(WBAuthorizeResponse *)response accessToken];
        
        DLog(@"userId %@",userId);
        DLog(@"accessToken %@",accessToken);
        if (userId != nil && accessToken != nil){
            DLog(@"新浪微博授权登录成功");
            NSDictionary *userInfo = @{@"userId" : userId, @"accessToken" : accessToken};
            [[NSNotificationCenter defaultCenter] postNotificationName:WeiboDidLoginNotification                                                            object:nil userInfo:userInfo];
        }else{
            [[NSNotificationCenter defaultCenter] postNotificationName:WeiboDidLoginNotification                                                            object:nil userInfo:nil];
        }
    }else if([response isKindOfClass:WBSendMessageToWeiboResponse.class]){//微博分享的回调
        WBSendMessageToWeiboResponse *res = (WBSendMessageToWeiboResponse *)response;
        if (res.statusCode == 0) {
            [ShareManage shareShareManager].successBlock();
        }else if (res.statusCode == -1){
            [ShareManage shareShareManager].failBlock(kUserCancel);
        }else{
            [ShareManage shareShareManager].failBlock(kErro);
        }
    }else if ([response isKindOfClass:[WBShareMessageToContactResponse class]]){//微博私信分享
        
        WBShareMessageToContactResponse *res = (WBShareMessageToContactResponse *)response;
        DLog(@"%ld",(long)res.statusCode);
        if (res.statusCode == 0) {
            [ShareManage shareShareManager].successBlock();
        }else if (res.statusCode == -1){
            [ShareManage shareShareManager].failBlock(kUserCancel);
        }else{
            [ShareManage shareShareManager].failBlock(kErro);
        }
    }
}
/**
 收到一个来自微博客户端程序的请求
 {
 
 收到微博的请求后，第三方应用应该按照请求类型进行处理，处理完后必须通过 [WeiboSDK sendResponse:] 将结果回传给微博
 @param request 具体的请求对象
 */
- (void)didReceiveWeiboRequest:(WBBaseRequest *)request{}

/*! 微信回调，不管是登录还是分享成功与否，都是走这个方法 @brief 发送一个sendReq后，收到微信的回应
 *
 * 收到一个来自微信的处理结果。调用一次sendReq后会收到onResp。
 * 可能收到的处理结果有SendMessageToWXResp、SendAuthResp等。
 * @param resp 具体的回应内容，是自动释放的
 */
-(void) onResp:(BaseResp*)resp{
    /*
     enum  WXErrCode {
     WXSuccess           = 0,    成功
     WXErrCodeCommon     = -1,  普通错误类型
     WXErrCodeUserCancel = -2,    用户点击取消并返回
     WXErrCodeSentFail   = -3,   发送失败
     WXErrCodeAuthDeny   = -4,    授权失败
     WXErrCodeUnsupport  = -5,   微信不支持
     };
     */
    if ([resp isKindOfClass:[SendAuthResp class]]) {   //授权登录的类。
        if (resp.errCode == 0) {  //成功。
            SendAuthResp *resp2 = (SendAuthResp *)resp;
            [[NSNotificationCenter defaultCenter] postNotificationName:WeChatDidLoginNotification object:nil userInfo:@{@"code":resp2.code}];
        }else{ //失败
            [[NSNotificationCenter defaultCenter] postNotificationName:WeChatDidLoginNotification object:nil userInfo:@{@"errCode":[NSString stringWithFormat:@"%d",resp.errCode]}];
        }
    }else if([resp isKindOfClass:[SendMessageToWXResp class]]){
        if (resp.errCode == 0) {  //成功。
            DLog(@"微信分享成功");
            [ShareManage shareShareManager].successBlock();
        }else{ //失败
            if (resp.errCode == -2) {
                DLog(@"用户取消WeChat分享");
                [ShareManage shareShareManager].failBlock(kUserCancel);
            }else{
                DLog(@"微信分享失败");
                [ShareManage shareShareManager].failBlock(kErro);
            }
        }
    }
}
-(void) onReq:(BaseReq*)req{}

@end
