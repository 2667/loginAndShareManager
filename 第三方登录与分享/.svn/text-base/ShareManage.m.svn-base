//
//  ShareManage.m
//  LoginAndShare
//
//  Created by 一公里 on 16/10/14.
//  Copyright © 2016年 第三方登录与分享. All rights reserved.
//

#import "ShareManage.h"
#import "LoginShareManage.h"
#import "ReturnImageTool.h"
static ShareManage *_shareManager;
@implementation ShareManage
+ (instancetype)shareShareManager{
    static dispatch_once_t predicate; dispatch_once(&predicate, ^{
        _shareManager = [[ShareManage alloc]init];
    });
    return _shareManager;
}
/**
 第三方分享
 @param url 视频内容的目标URL
 @param title 分享内容的标题
 @param description 分享内容的描述
 @param imageUrl 分享内容的预览图像
 @param successBlock 分享成功返回
 @param failBlock 分享失败返回
 */
+ (void)shareType:(kLoginShareType)type url:(NSString *)url title:(NSString *)title  description:(NSString *)description imageUrl:(NSString *)imageUrl successBlock:(ShareSuccessBlock)successBlock failBlock:(ShareFailBlock)failBlock{
    DLog(@"\nURL=%@\n title = %@ \n description = %@ \n imageUrl = %@",url,title,description,imageUrl);
    
    
    [ShareManage shareShareManager].successBlock = successBlock;
    [ShareManage shareShareManager].failBlock = failBlock;

    switch (type) {
        case kQQ:
            [ShareManage shareQQUrl:url title:title description:description imageUrl:imageUrl];
            break;
        case kQQZone:
            [ShareManage shareQQZoneUrl:url title:title description:description imageUrl:imageUrl];
            break;
        case kSina:
            [ShareManage shareSinaType:1  Url:url title:title description:description imageUrl:imageUrl];
            break;
        case kSinaFriend:
            [ShareManage shareSinaType:0  Url:url title:title description:description imageUrl:imageUrl];
            break;
        case kWeChatFriend:
            [ShareManage shareWeChatType:0 Url:url title:title description:description imageUrl:imageUrl];
            break;
        case kWeChatFriendCircle:
            [ShareManage shareWeChatType:1 Url:url title:title description:description imageUrl:imageUrl];
            break;
        case kWeChatCollect:
            [ShareManage shareWeChatType:2 Url:url title:title description:description imageUrl:imageUrl];
            
            break;
        default:
            break;
    }
    
}

/**
 分享到QQ
 */
+ (void)shareQQUrl:(NSString *)url title:(NSString *)title  description:(NSString *)description imageUrl:(NSString *)imageUrl{
    if (![TencentOAuth iphoneQQInstalled]) {
        [ShareManage shareShareManager].failBlock(kErro);
        DLog(@"未安装qq");
        return;
    }
    static dispatch_once_t predicate; dispatch_once(&predicate, ^{
        [LoginShareManage TencentWithDelegate:nil];
    });
    ShareManage *manage = [ShareManage shareShareManager];
    [QQApiInterface sendReq:[manage shareUrl:url title:title description:description imageUrl:imageUrl]];
}
/**
 分享到QQ空间
 */
+ (void)shareQQZoneUrl:(NSString *)url title:(NSString *)title  description:(NSString *)description imageUrl:(NSString *)imageUrl{
    if (![TencentOAuth iphoneQQInstalled] && ![TencentOAuth iphoneQZoneInstalled]) {
        [ShareManage shareShareManager].failBlock(kErro);
        DLog(@"未安装QQ与QQ空间");
        return;
    }
    static dispatch_once_t predicate; dispatch_once(&predicate, ^{
        [LoginShareManage TencentWithDelegate:nil];
    });
    ShareManage *manage = [ShareManage shareShareManager];
    [QQApiInterface SendReqToQZone:[manage shareUrl:url title:title description:description imageUrl:imageUrl]];
}
/**
 分享到新浪微博
 @param type 0 = 好友列表 1 = 发微博
 */
+ (void)shareSinaType:(int)type Url:(NSString *)url title:(NSString *)title  description:(NSString *)description imageUrl:(NSString *)imageUrl{
    [LoginShareManage SinaWeibo];
    
    WBMessageObject *message = [WBMessageObject message];
    message.text = [NSString stringWithFormat:@"%@  %@ \n(分享自@二三里News)",title,url];
    NSData *imageData = UIImageJPEGRepresentation([UIImage imageWithData:[NSData dataWithContentsOfURL:[NSURL URLWithString:imageUrl]]], 0.1);
    if (imageData) {
        WBImageObject *imgObject = [WBImageObject object];
        imgObject.imageData = imageData;
        message.imageObject = imgObject;
    }

    if (type == 0) {
        WBShareMessageToContactRequest *request = [WBShareMessageToContactRequest requestWithMessage:message];
        [WeiboSDK sendRequest:request];

    }else{
        WBSendMessageToWeiboRequest *request = [WBSendMessageToWeiboRequest requestWithMessage:message authInfo:[WBBaseResponse response] access_token:nil];
        [WeiboSDK sendRequest:request];
    }
}
/**
 分享到微信
 @param type 0 = 好友列表 1 = 朋友圈 2 = 收藏
 */
+ (void)shareWeChatType:(int)type Url:(NSString *)url title:(NSString *)title  description:(NSString *)description imageUrl:(NSString *)imageUrl{
    [LoginShareManage WeChat];
    if (![[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:@"wechat://"]]) {
        [ShareManage shareShareManager].failBlock(kErro);
        DLog(@"微信未安装");
        return;
    }
    
    //创建发送对象实例
    SendMessageToWXReq *sendReq = [[SendMessageToWXReq alloc] init];
    sendReq.bText = NO;//不使用文本信息
    sendReq.scene = type;//0 = 好友列表 1 = 朋友圈 2 = 收藏
    
    //创建分享内容对象
    WXMediaMessage *urlMessage = [WXMediaMessage message];
    urlMessage.title = title;//分享标题
    urlMessage.description = description;//分享描述
    UIImage *img = [ReturnImageTool handleImageWithURLStr:imageUrl];//压缩图片的拓展方法
    NSData *imgData = UIImageJPEGRepresentation(img, 0.3);
    [urlMessage setThumbData:imgData];//imgData不能超过32k
    
    //创建多媒体对象
    WXWebpageObject *webObj = [WXWebpageObject object];
    webObj.webpageUrl = url;//分享链接
    
    //完成发送对象实例
    urlMessage.mediaObject = webObj;
    sendReq.message = urlMessage;
    
    //发送分享信息
    [WXApi sendReq:sendReq];
}

/**
 分享到QQ与QQ空间配置
 */
- (SendMessageToQQReq *)shareUrl:(NSString *)url title:(NSString *)title  description:(NSString *)description imageUrl:(NSString *)imageUrl{
    
    UIImage *img = [ReturnImageTool handleImageWithURLStr:imageUrl];//压缩图片的拓展方法
    NSData *imgData = UIImageJPEGRepresentation(img, 0.3);
    
    QQApiNewsObject *newsObj = [QQApiNewsObject objectWithURL:[NSURL URLWithString:url]
                                                title:title
                                                description:description
                                                previewImageData:imgData];
    
    SendMessageToQQReq *req = [SendMessageToQQReq reqWithContent:newsObj];
    
    return req;
}


/**
 处理来至QQ的响应
 */
- (void)onResp:(QQBaseResp *)resp{
    
    if ([resp.class isSubclassOfClass: [SendMessageToQQResp class]]) {  //QQ分享回应
        SendMessageToQQResp *resps = (SendMessageToQQResp *)resp;
        NSString *result = resps.result;
        DLog(@"--%@--",result);
        if ([resps.result isEqualToString:@"-4"]) {
            DLog(@"用户取消QQ分享");
            [ShareManage shareShareManager].failBlock(kUserCancel);
        }else if ([resps.result isEqualToString:@"0"]){
            DLog(@"QQ分享成功");
            [ShareManage shareShareManager].successBlock();
        }else{
            DLog(@"QQ分享失败");
            [ShareManage shareShareManager].failBlock(kErro);
        }
    }
}
/**
 处理来至QQ的请求
 */
- (void)onReq:(QQBaseReq *)req{}
/**
 处理QQ在线状态的回调
 */
- (void)isOnlineResponse:(NSDictionary *)response{}

@end
