/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package com.rhz.game;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxHelper;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.Process;
import android.widget.Toast;

import com.multimode_billing_sms.ui.MultiModePay;
import com.multimode_billing_sms.ui.MultiModePay.SMSCallBack;

public class WestMiao extends Cocos2dxActivity {
	private static WestMiao instance;
	private static boolean showDlg;

	private static String[][] code = {
			{
					"130909010543",
					"购买正式版" },
			{
					"130909010544",
					"购买道具月牙杖" },
			{
					"130909010545",
					"购买道具九齿耙" },
			{
					"130909010546",
					"购买道具金箍棒" },
			{
					"130909010547",
					"购买道具西装" } };

	private static Handler handle = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			final int chargeNum = msg.what;
			if (chargeNum == 999) {
				new AlertDialog.Builder(instance).setMessage("确定要退出游戏吗？")
						.setNegativeButton("取消", null)
						.setPositiveButton("确定", new OnClickListener() {

							@Override
							public void onClick(DialogInterface arg0, int arg1) {
								// TODO Auto-generated method stub
								Cocos2dxHelper.end();
								Process.killProcess(Process.myPid());
							}
						}).show();
			} else {
				MultiModePay.getInstance().sms(instance, "合肥优点信息技术有限公司", "021-65879070", "悟空救我之齐天大圣",
							code[chargeNum - 1][1], "2",code[chargeNum - 1][0], new SMSCallBack(){

								@Override
								public void ButtonCLick(int arg0) {
									// TODO Auto-generated method stub
									
								}

								@Override
								public void SmsResult(int arg0, String arg1) {
									// TODO Auto-generated method stub
										System.out.println(arg0);
										System.out.println(arg1);
										MultiModePay.getInstance().DismissProgressDialog(); 
										if(arg0 == MultiModePay.CANCEL || arg0 == MultiModePay.FAILD){
											if (chargeNum > 1) {
												JniCall.callCMethod(-1);
											} else if (chargeNum == 1) {
												JniCall.callCMethod(-2);
											}
											showDlg = false;
											if(arg0 == MultiModePay.FAILD){
												Toast.makeText(instance, "支付失败，请稍候重试，谢谢", Toast.LENGTH_SHORT).show();
											}
										}else if(arg0 == MultiModePay.SUCCESS){
											JniCall.callCMethod(chargeNum);
											showDlg = false;
											Toast.makeText(instance, "支付成功，祝您游戏愉快", Toast.LENGTH_SHORT).show();
										}
								}
					
				});
//				if (SMS.checkFee("game_00" + chargeNum, instance,
//						new SMSListener() {
//							//
//							@Override
//							public void smsOK(String arg0) {
//								JniCall.callCMethod(chargeNum);
//								showDlg = false;
//							}
//
//							@Override
//							public void smsFail(String arg0, int arg1) {
////								if(chargeNum == 1){
////									JniCall.callCMethod(-2);
////								}
//								showDlg = false;
//							}
//
//							@Override
//							public void smsCancel(String arg0, int arg1) {
//								if (chargeNum > 1) {
//									JniCall.callCMethod(-1);
//								} else if (chargeNum == 1) {
//									JniCall.callCMethod(-2);
//								}
//								showDlg = false;
//							}
//						}, code[chargeNum - 1][0], code[chargeNum - 1][1]
//								+ ",发送一条2元短信，不含信息费", "购买成功，祝您 游戏愉快！~", false)) {
//				} else {
//					System.out.println("" + SMS.getResult());
//				}
			}
		}
	};

	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		instance = this;
		MultiModePay.getInstance().setEnableSend(true);	
	}

	public static void exitGame() {
		handle.sendEmptyMessage(999);
	}

	public static boolean doCharge(final int chargeNum) {
		System.out.println("the charge point is 00" + chargeNum);
		if (chargeNum == -1) {
			exitGame();
			return false;
		} else if(chargeNum == -2){
			Uri uri = Uri.parse("http://store.wo.com.cn/");
			Intent intent = new Intent(Intent.ACTION_VIEW,uri);
			instance.startActivity(intent);
			return false;
		}else {
			if (!showDlg) {
				handle.sendEmptyMessage(chargeNum);
				showDlg = true;
			}
			return false;
		}
	}

	public static boolean checkPay(int index) {
		if (index == -1) { // 用来获取是否开启声音，同用此函数
			return true;
		} else {
			return doCharge(index);
		}
	}

	static {
		System.loadLibrary("game");
	}

}
