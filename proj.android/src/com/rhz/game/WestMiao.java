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

import cn.game189.sms.SMS;
import cn.game189.sms.SMSListener;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.Process;
import android.widget.Toast;

public class WestMiao extends Cocos2dxActivity{
	private static WestMiao instance;
	private static String[] code = {
		"0211C0945611022216975111022216906001MC090000000000000000000000000000",
		"0211C0945611022216975111022216906101MC090000000000000000000000000000",
		"0211C0945611022216975111022216906201MC090000000000000000000000000000",
		"0211C0945611022216975111022216905801MC090000000000000000000000000000",
		"0211C0945611022216975111022216905901MC090000000000000000000000000000"
	};
	
	private static Handler handle = new Handler(){
		@Override
		public void handleMessage(Message msg){
			System.out.println("++++++++++++++++");
			final int chargeNum = msg.what;
					 if(SMS.checkFee("game_00" + chargeNum, instance, new SMSListener() {
//				
				@Override
				public void smsOK(String arg0) {
					JniCall.callCMethod(chargeNum);
				}
				
				@Override
				public void smsFail(String arg0, int arg1) {
					
					Toast.makeText(instance, arg0, Toast.LENGTH_LONG).show();
				}
				
				@Override
				public void smsCancel(String arg0, int arg1) {
					if(chargeNum > 1){
						JniCall.callCMethod(-1);
					}else if(chargeNum == 1){
						//JniCall.callCMethod(-2);
					}
					
				}
			}, code[chargeNum - 1] , "购买 道具，发送一条2元短信，不含信息费", "购买成功，祝您 游戏愉快！~", false)){
			 }else{
				System.out.println("--------------------" + SMS.getResult()); 
				
			 }
		}
		
	};

	protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		instance = this;
		SMS.gameStart(this);
	} 
	public static void exitGame() {
		SMS.gameExit(instance);
	
	}
	
	public static boolean doCharge(final int chargeNum){
		System.out.println("the charge point is 00" + chargeNum);
		if(chargeNum == -1){
			exitGame();
			return false;
		}else{
//			 if(SMS.checkFee("game_00" + chargeNum, instance, new SMSListener() {
//				
//				@Override
//				public void smsOK(String arg0) {
//					JniCall.callCMethod(chargeNum);
//				}
//				
//				@Override
//				public void smsFail(String arg0, int arg1) {
//					
//					Toast.makeText(instance, arg0, Toast.LENGTH_LONG).show();
//				}
//				
//				@Override
//				public void smsCancel(String arg0, int arg1) {
//					if(chargeNum > 1){
//						JniCall.callCMethod(-1);
//					}else if(chargeNum == 1){
//						JniCall.callCMethod(-2);
//					}
//					
//				}
//			}, code[chargeNum - 1] , "购买 道具，发送一条2元短信，不含信息费", "购买成功，祝您 游戏愉快！~", false)){
//				 return true;
//			 }else{
//				System.out.println("--------------------" + SMS.getResult()); 
//				
//				return false;
//			 }
			handle.sendEmptyMessage(chargeNum);
			return false;
		}
	}
	
	public static boolean checkPay(int index){
		if(index == -1){  //用来获取是否开启声音，同用此函数
			return true;
		}else{
			return doCharge(index);
		}
	}
	
    static {
         System.loadLibrary("game");
    }
    
    
}
