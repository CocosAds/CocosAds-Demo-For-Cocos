//
//  CocosAds-android.cpp
//  CocosAds
//
//  Created by Jacky on 16/2/15.
//
//

#include "CocosAds.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <cocos2d.h>
#define CLASS_NAME "com/cocos/ads/helper/CocosAdsHelper"

using namespace cocos2d;
using namespace std;

#pragma mark - CocosAds

CocosAds* CocosAds::_instance = nullptr;

CocosAds::CocosAds()
{
    _impl = new CocosAdsImpl(this);
}

CocosAds* CocosAds::getInstance()
{
    if(_instance == nullptr)
    {
        _instance = new CocosAds();
    }
    return _instance;
}

void CocosAds::init(const char* publisherID)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "init", "(Ljava/lang/String;)V"))
    {
        jstring stringArg1 = t.env->NewStringUTF(publisherID);
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1);
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(stringArg1);
    }
}

#pragma mark - Banner

void CocosAds::showBanner(const char* placementID /*= ""*/)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "showBanner", "(Ljava/lang/String;)V"))
    {
        jstring stringArg1 = t.env->NewStringUTF(placementID);
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1);
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(stringArg1);
    }
}

void CocosAds::hideBanner()
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "hideBanner", "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}

void CocosAds::setBannerOnReceiveAdSuccess(const std::function<void()> &callback)
{
    _bannerOnReceiveAdSuccess = callback;
}

void CocosAds::setBannerOnReceiveAdFailed(const std::function<void(const std::string& errMsg)> &callback)
{
    _bannerReceiveAdFailed = callback;
}

void CocosAds::setBannerOnPresentScreen(const std::function<void()> &callback)
{
    _bannerOnPresentScreen = callback;
}

void CocosAds::setBannerOnDismissScreen(const std::function<void()> &callback)
{
    _bannerOnDismissScreen = callback;
}

#pragma mark - Interstitial

void CocosAds::showInterstitial(const char* placementID /*= ""*/)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "showInterstitial", "(Ljava/lang/String;)V"))
    {
        jstring stringArg1 = t.env->NewStringUTF(placementID);
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1);
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(stringArg1);
    }
}

void CocosAds::setInterstitialCloseMode(int closeMode)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "setInterstitialCloseMode", "(I)V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, closeMode);
        t.env->DeleteLocalRef(t.classID);
    }
}

void CocosAds::setInterstitialDisplayTime(int seconds)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "setInterstitialDisplayTime", "(I)V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, seconds);
        t.env->DeleteLocalRef(t.classID);
    }
}

void CocosAds::hideInterstitial()
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "hideInterstitial", "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}

#pragma mark - JNI

extern "C" {
    JNIEXPORT void JNICALL Java_com_cocos_ads_helper_CocosAdsHelper_bannerReceiveAdSuccess(JNIEnv *env, jclass) {
        return CocosAdsImpl::bannerReceiveAdSuccess();
    }
    
    JNIEXPORT void JNICALL Java_com_cocos_ads_helper_CocosAdsHelper_bannerReceiveAdFailed(JNIEnv *env, jclass, jstring errMsg) {
        auto charErrMsg = env->GetStringUTFChars(errMsg, nullptr);
        std::string err = charErrMsg;
        env->ReleaseStringUTFChars(errMsg, charErrMsg);
        return CocosAdsImpl::bannerReceiveAdFailed(err);
    }
    
    JNIEXPORT void JNICALL Java_com_cocos_ads_helper_CocosAdsHelper_bannerPresentScreen(JNIEnv *env, jclass) {
        return CocosAdsImpl::bannerPresentScreen();
    }
    
    JNIEXPORT void JNICALL Java_com_cocos_ads_helper_CocosAdsHelper_bannerDismissScreen(JNIEnv *env, jclass) {
        return CocosAdsImpl::bannerDismissScreen();
    }
}

#pragma mark - CocosAdsImpl

CocosAds* CocosAdsImpl::_cocosads = nullptr;

CocosAdsImpl::CocosAdsImpl(CocosAds* cocosads)
{
    _cocosads = cocosads;
}

void CocosAdsImpl::bannerReceiveAdSuccess()
{
    if (_cocosads->_bannerOnReceiveAdSuccess)
    {
        _cocosads->_bannerOnReceiveAdSuccess();
    }
}

void CocosAdsImpl::bannerReceiveAdFailed(const std::string errMsg)
{
    if(_cocosads->_bannerReceiveAdFailed)
    {
        _cocosads->_bannerReceiveAdFailed(errMsg);
    }
}
void CocosAdsImpl::bannerPresentScreen()
{
    if(_cocosads->_bannerOnPresentScreen)
    {
        _cocosads->_bannerOnPresentScreen();
    }
}

void CocosAdsImpl::bannerDismissScreen()
{
    if(_cocosads->_bannerOnDismissScreen)
    {
        _cocosads->_bannerOnDismissScreen();
    }
}

#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID