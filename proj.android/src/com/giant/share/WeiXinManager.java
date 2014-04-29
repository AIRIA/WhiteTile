package com.giant.share;

import java.io.File;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.util.Log;

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

	public static void init(Context context) {
		mContext = context;
		reg2WX();
	}

	public static void reg2WX() {
		api = WXAPIFactory.createWXAPI(mContext, APP_ID, true);
		api.registerApp(APP_ID);
	}

	public static void shareImage(final String path) {
		Activity act = (Activity) mContext;
		act.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				File file = new File(path);
				if (!file.exists()) {
					// Toast.makeText(mContext," path = " + path,
					// Toast.LENGTH_LONG).show();
					return;
				} else {
					// Toast.makeText(mContext," finded path = " + path,
					// Toast.LENGTH_LONG).show();
				}

				WXImageObject imgObj = new WXImageObject();
				imgObj.setImagePath(path);

				WXMediaMessage msg = new WXMediaMessage();
				msg.mediaObject = imgObj;

				Bitmap bmp = BitmapFactory.decodeFile(path);
				Bitmap thumbBmp = Bitmap.createScaledBitmap(bmp, 320, 480, true);
				bmp.recycle();
				msg.thumbData = Util.bmpToByteArray(thumbBmp, true);

				SendMessageToWX.Req req = new SendMessageToWX.Req();
				req.transaction = buildTransaction("img");
				req.message = msg;
				req.scene = SendMessageToWX.Req.WXSceneSession;
				Boolean res = api.sendReq(req);
				Log.v("cocos2d-x", res.toString());
			}
		});

	}

	private static String buildTransaction(final String type) {
		return (type == null) ? String.valueOf(System.currentTimeMillis()) : type + System.currentTimeMillis();
	}
	
	public static void mark(){
		Uri uri = Uri.parse("market://details?id=com.giant.whitetile");
		Intent markIntent = new Intent(Intent.ACTION_VIEW,uri);
		mContext.startActivity(Intent.createChooser(markIntent, "选择应用程序"));
	}
	
}
