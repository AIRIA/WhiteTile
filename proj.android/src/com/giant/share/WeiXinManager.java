package com.giant.share;

import java.io.File;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;
import android.widget.Toast;

import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.SendMessageToWX;
import com.tencent.mm.sdk.openapi.WXAPIFactory;
import com.tencent.mm.sdk.openapi.WXImageObject;
import com.tencent.mm.sdk.openapi.WXMediaMessage;
import com.tencent.mm.sdk.platformtools.Util;

public class WeiXinManager {

	private static final String APP_ID = "wx61e52e13d5f5e847";
	private static IWXAPI api;
	private static Context mContext;
	
	public static void init(Context context)
	{
		mContext = context;
		reg2WX();
	}
	
	public static void reg2WX() {
		api = WXAPIFactory.createWXAPI(mContext,APP_ID,true);
		api.registerApp(APP_ID);
	}
	private static final int THUMB_SIZE = 150;
	
	public static void shareImage(final String path){
		Activity act = (Activity)mContext;
		act.runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				File file = new File(path);
				if (!file.exists()) {
					Toast.makeText(mContext," path = " + path, Toast.LENGTH_LONG).show();
					return;
				}else{
					Toast.makeText(mContext," finded path = " + path, Toast.LENGTH_LONG).show();
				}
				
				WXImageObject imgObj = new WXImageObject();
				imgObj.setImagePath(path);
				
				WXMediaMessage msg = new WXMediaMessage();
				msg.mediaObject = imgObj;
				
				Bitmap bmp = BitmapFactory.decodeFile(path);
				Bitmap thumbBmp = Bitmap.createScaledBitmap(bmp, THUMB_SIZE, THUMB_SIZE, true);
				bmp.recycle();
				msg.thumbData = Util.bmpToByteArray(thumbBmp, true);
				
				SendMessageToWX.Req req = new SendMessageToWX.Req();
				req.transaction = buildTransaction("img");
				req.message = msg;
				req.scene = SendMessageToWX.Req.WXSceneTimeline;
				Boolean res= api.sendReq(req);
				Log.v("cocos2d-x",res.toString());
			}
		});
		
	}
	
	private static String buildTransaction(final String type) {
		return (type == null) ? String.valueOf(System.currentTimeMillis()) : type + System.currentTimeMillis();
	}
}
