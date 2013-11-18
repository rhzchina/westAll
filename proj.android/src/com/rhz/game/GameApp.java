package com.rhz.game;

import android.app.Application;

public class GameApp extends Application{
	@Override
	public void onCreate(){
	  System.loadLibrary("megjb");
	}
}