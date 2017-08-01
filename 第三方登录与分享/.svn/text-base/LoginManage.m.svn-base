//
//  LoginManage.m
//  LoginAndShare
//
//  Created by 一公里 on 16/10/14.
//  Copyright © 2016年 第三方登录与分享. All rights reserved.
//

#import "LoginManage.h"
#import "LoginShareManage.h"

@interface LoginManage ()<TencentSessionDelegate>
@property (nonatomic, copy) UserInfo userInfo;
@property (nonatomic, copy) LoginFail loginFail;
@end

static LoginManage *_loginManager;
@implementation LoginManage
+ (instancetype)shareLoginManage{
    
    static dispatch_once_t predicate; dispatch_once(&predicate, ^{
        _loginManager = [[LoginManage alloc] init];
    });
    return _loginManager;
}
/**
 第三方授权登录授权登录
 @param type 传入登录方式 (QQ 新浪 微信)
 @param userInfo 登录成功后用户的基本信息
 */
+ (void)login:(kLoginShareType)type userInfo:(UserInfo)userInfo loginFail:(LoginFail)fail{

    [LoginManage shareLoginManage].userInfo = userInfo;
    [LoginManage shareLoginManage].loginFail = fail;
    switch (type) {
        case kQQ:
            [LoginManage loginQQ];
            break;
        case kWeChat:
            [LoginManage loginWeChat];
            break;
        case kSina:
            [LoginManage loginSina];
            break;
        
        default:
            DLog(@"第三方登录类型不正确!!!");
            break;
    }
}


/*
 QQ授权登录
 **/
+ (void)loginQQ{
    static dispatch_once_t predicate; dispatch_once(&predicate, ^{
        [LoginShareManage TencentWithDelegate:[LoginManage shareLoginManage]];
    });
    LoginShareManage *manage = [LoginShareManage shareLoginShareManage];
    [manage.tencentOAuth authorize:manage.permissions inSafari:NO];
}

/*
 新浪微博授权登录
 **/
+ (void)loginSina{
    [LoginShareManage SinaWeibo];
    [[NSNotificationCenter defaultCenter] addObserver:[LoginManage shareLoginManage] selector:@selector(weiboDidLoginNotification:) name:WeiboDidLoginNotification object:nil];
    
    WBAuthorizeRequest *request = [WBAuthorizeRequest request];
    request.redirectURI = SINA_BACK_URL;
    request.scope = @"all";
    request.userInfo = @{@"SSO_From": @"SendMessageToWeiboViewController",
                         @"Other_Info_1": [NSNumber numberWithInt:123],
                         @"Other_Info_2": @[@"obj1", @"obj2"],
                         @"Other_Info_3": @{@"key1": @"obj1", @"key2": @"obj2"}};
    [WeiboSDK sendRequest:request];
}
/*
 微信授权登录
 **/
+ (void)loginWeChat{
    [LoginShareManage WeChat];
    if ([WXApi isWXAppInstalled]) {
        [[NSNotificationCenter defaultCenter] addObserver:[LoginManage shareLoginManage] selector:@selector(weChatDidLoginNotification:) name:WeChatDidLoginNotification object:nil];

        SendAuthReq *req = [[SendAuthReq alloc]init];
        req.scope = @"snsapi_userinfo";
        req.openID = WECHAT_APP_ID;
        req.state = @"1245";
        [WXApi sendReq:req];
    }
}

#pragma mark QQ登录回调
/**
 QQ授权登录回调
 */
- (void)tencentDidLogin{
    LoginShareManage *manage = [LoginShareManage shareLoginShareManage];
    if (manage.tencentOAuth.accessToken && 0!=[manage.tencentOAuth.accessToken length]) {
        DLog(@"QQ登录成功");
        DLog(@"token=====%@---openID ===== %@",manage.tencentOAuth.accessToken,manage.tencentOAuth.openId);
        [LoginManage shareLoginManage].userInfo(nil,nil,nil,manage.tencentOAuth.openId,manage.tencentOAuth.accessToken,nil);
        return;
        //获取用户详细信息
//        [manage.tencentOAuth getUserInfo];
        
    }else{
        DLog(@"QQ获取登录信息失败");
        [LoginManage shareLoginManage].loginFail(kErro);
    }
}
- (void)tencentDidNotLogin:(BOOL)cancelled{
    if (cancelled) {
        DLog(@"用户取消QQ登录");
        [LoginManage shareLoginManage].loginFail(kUserCancel);
    }else{
        DLog(@"QQ登录失败");
        [LoginManage shareLoginManage].loginFail(kErro);
    }
}
- (void)tencentDidNotNetWork{
    DLog(@"网络出错");
    [LoginManage shareLoginManage].loginFail(kErro);
}
/**
 QQ用户信息获取delegate
 */
- (void)getUserInfoResponse:(APIResponse *)response{
    //qq头像:figureurl_qq_2 qq昵称:nickname 性别:gender openId:[LoginShareManage shareLoginShareManage].tencentOAuth.openId
    DLog(@"获取用户信息:%@",response.message);
    
    NSDictionary *dict = [NSJSONSerialization JSONObjectWithData:[response.message dataUsingEncoding:NSUTF8StringEncoding] options:NSJSONReadingMutableContainers error:nil];
    LoginShareManage *manager = [LoginShareManage shareLoginShareManage];
    [LoginManage shareLoginManage].userInfo(dict[@"nickname"],dict[@"figureurl_qq_2"],([dict[@"gender"] isEqualToString:@"男"] ? @"1" : @"0"),manager.tencentOAuth.openId,manager.tencentOAuth.accessToken,@"");
}
#pragma mark 新浪微博登录回调
/**
 新浪微博登录成功回调通知
 */
- (void)weiboDidLoginNotification:(NSNotification *)not{
    [[NSNotificationCenter defaultCenter]removeObserver:[LoginManage shareLoginManage] name:WeiboDidLoginNotification object:nil];
    if (not.userInfo != nil) {
        
        NSString *accessToken = [not.userInfo objectForKey:@"accessToken"];
        NSString *uid = [not.userInfo objectForKey:@"userId"];
        
        [LoginManage shareLoginManage].userInfo(nil,nil,nil,uid,accessToken,nil);
        return;
        //获取用户详细信息
//        [self getWeiboUserInfoWithAccessToken:accessToken uid:uid];
    }else{
        [LoginManage shareLoginManage].loginFail(kErro);
    }
}
/**
 新浪微博获取用户信息
 */
- (void)getWeiboUserInfoWithAccessToken:(NSString *)accessToken uid:(NSString *)uid
{
    NSString *url =[NSString stringWithFormat:
                    @"https://api.weibo.com/2/users/show.json?access_token=%@&uid=%@",accessToken,uid];
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        NSURL *zoneUrl = [NSURL URLWithString:url];
        NSString *zoneStr = [NSString stringWithContentsOfURL:zoneUrl
                                                     encoding:NSUTF8StringEncoding error:nil];
        NSData *data = [zoneStr dataUsingEncoding:NSUTF8StringEncoding];
        dispatch_async(dispatch_get_main_queue(), ^{
            if (data)
            {
                NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:data
                                                                    options:NSJSONReadingMutableContainers error:nil];
                DLog(@"%@",dic);
                if (dic[@"name"] != nil && dic[@"profile_image_url"] != nil && dic[@"gender"] != nil) {
                    [LoginManage shareLoginManage].userInfo(dic[@"name"],dic[@"profile_image_url"],([dic[@"gender"] isEqualToString:@"m"] ? @"1" : @"0"),uid,accessToken,@"");
                }else{
                    [LoginManage shareLoginManage].loginFail(kErro);
                }
            }else{
                [LoginManage shareLoginManage].loginFail(kErro);
            }
        });
        
    });
}


#pragma mark 微信登录回调
/**
 根据返回的code值请求openId与access_token
 */
-(void)weChatDidLoginNotification:(NSNotification *)noti{
    
    [[NSNotificationCenter defaultCenter]removeObserver:[LoginManage shareLoginManage] name:WeChatDidLoginNotification object:nil];
    if (noti.userInfo[@"errCode"] != nil) {
        if ([noti.userInfo[@"errCode"] isEqualToString:@"-2"]) {
            DLog(@"用户取消WeChat登录");
            [LoginManage shareLoginManage].loginFail(kUserCancel);
        }else{
            DLog(@"WeChat登录失败");
            [LoginManage shareLoginManage].loginFail(kErro);
        }
        return;
    }
    
    [LoginManage shareLoginManage].userInfo(nil,nil,nil,nil,nil,noti.userInfo[@"code"]);

    return;
    //获取access_token与openId
//    __weak typeof(*&self) weakSelf = self;
//    NSString *url =[NSString stringWithFormat:@"https://api.weixin.qq.com/sns/oauth2/access_token?appid=%@&secret=%@&code=%@&grant_type=authorization_code",WECHAT_APP_ID,WECHAT_APP_SECRET,noti.userInfo[@"code"]];
//    NSLog(@"%@",url);
//    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
//        NSURL *zoneUrl = [NSURL URLWithString:url];
//        NSString *zoneStr = [NSString stringWithContentsOfURL:zoneUrl
//                                                     encoding:NSUTF8StringEncoding error:nil];
//        NSData *data = [zoneStr dataUsingEncoding:NSUTF8StringEncoding];
//        dispatch_async(dispatch_get_main_queue(), ^{
//            if (data)
//            {
//                NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:data
//                                                                    options:NSJSONReadingMutableContainers error:nil];
//                NSLog(@"%@",dic);
//                /*
//                 access_token   接口调用凭证
//                 expires_in access_token接口调用凭证超时时间，单位（秒）
//                 refresh_token  用户刷新access_token
//                 openid 授权用户唯一标识
//                 scope  用户授权的作用域，使用逗号（,）分隔
//                 unionid     当且仅当该移动应用已获得该用户的userinfo授权时，才会出现该字段
//                 */
//                if (dic[@"access_token"] && dic[@"openid"]) {
//                    [weakSelf requestUserInfoByToken:dic[@"access_token"] andOpenid:dic[@"openid"]];
//                }else{
//                    NSLog(@"WeChat登录失败");
//                    [LoginManage shareLoginManage].loginFail(kErro);
//                }
//            }else{
//                NSLog(@"WeChat登录失败");
//                [LoginManage shareLoginManage].loginFail(kErro);
//            }
//        });
//        
//    });
    
}
/**
 微信获取用户信息
 */
-(void)requestUserInfoByToken:(NSString *)token andOpenid:(NSString *)openID{
    
    NSString *url = [NSString stringWithFormat:@"https://api.weixin.qq.com/sns/userinfo?access_token=%@&openid=%@",token,openID];
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        NSURL *zoneUrl = [NSURL URLWithString:url];
        NSString *zoneStr = [NSString stringWithContentsOfURL:zoneUrl
                                                     encoding:NSUTF8StringEncoding error:nil];
        NSData *data = [zoneStr dataUsingEncoding:NSUTF8StringEncoding];
        dispatch_async(dispatch_get_main_queue(), ^{
            if (data)
            {
                NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:data
                                                                    options:NSJSONReadingMutableContainers error:nil];
                DLog(@"%@",dic);
                //headimgurl   nickname   openid  sex
                if (dic[@"nickname"] && dic[@"sex"] && dic[@"headimgurl"] && dic[@"openid"]) {
                    [LoginManage shareLoginManage].userInfo(dic[@"nickname"],dic[@"sex"],dic[@"headimgurl"],openID,token,@"");
                }else{
                    DLog(@"WeChat登录失败");
                    [LoginManage shareLoginManage].loginFail(kErro);
                }
            }else{
                DLog(@"WeChat登录失败");
                [LoginManage shareLoginManage].loginFail(kErro);
            }
        });
        
    });
}


@end
