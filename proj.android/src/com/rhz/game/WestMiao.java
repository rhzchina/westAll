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

import cn.cmgame.billing.api.GameInterface;
import cn.cmgame.billing.api.GameInterface.BillingCallback;

import android.os.Bundle;
import android.os.Process;
import android.widget.ProgressBar;
import android.widget.Toast;

public class WestMiao extends Cocos2dxActivity{
	private static WestMiao instance;

	protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		instance = this;
		GameInterface.initializeApp(this);
	}
	
	
	public static void exitGame() {
		GameInterface.exit(instance, new GameInterface.GameExitCallback() {
			@Override
			public void onConfirmExit() {
				Cocos2dxHelper.end();
				Process.killProcess(android.os.Process.myPid());
			}
	
			@Override
			public void onCancelExit() {
			}
		});
	}
	
	public static void doCharge(final int chargeNum){
		System.out.println("the charge point is 00" + chargeNum);
		if(chargeNum == -1){
			exitGame();
		}else if(chargeNum == -2){  //更多游戏
			
		}else{
			GameInterface.doBilling(instance, true, false, "00" + chargeNum, new BillingCallback() {
				
				@Override
				public void onUserOperCancel(String arg0) {
					if(chargeNum > 1){
						JniCall.callCMethod(-1);
					}else if(chargeNum == 1){
						JniCall.callCMethod(-2);
					}
				}
				
				@Override
				public void onBillingSuccess(String arg0) {
					JniCall.callCMethod(chargeNum);
					Toast.makeText(instance, "购买成功，祝您游戏愉快!~", Toast.LENGTH_LONG).show();
				}
				
				@Override
				public void onBillingFail(String arg0) {
					Toast.makeText(instance, arg0, Toast.LENGTH_LONG).show();
					
				}
			});
		}
	}
	
	public static boolean checkPay(int index){
		if(index == -1){  //用来获取是否开启声音，同用此函数
			return GameInterface.isMusicEnabled();
		}else{
			return GameInterface.getActivateFlag("00" + index);
		}
	}
	
    static {
         System.loadLibrary("game");
    }
    
    
}
