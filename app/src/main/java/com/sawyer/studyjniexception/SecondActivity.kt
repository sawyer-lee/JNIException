package com.sawyer.studyjniexception

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.util.NoSuchPropertyException
import com.sawyer.studyjniexception.databinding.ActivitySecondBinding
import java.lang.Exception

class SecondActivity : AppCompatActivity() {

    companion object {
        init {
            System.loadLibrary("studyjniexception")
        }
        // 假设这里定义了一大堆静态变量
        @JvmStatic
        var name111 = "T1"
        @JvmStatic
        external fun exception1()
        @JvmStatic
        @Throws(NoSuchFieldException::class)
        external fun exception2()
        @JvmStatic
        external fun exception3()

        //示例：Java层出现了异常代码，C++层帮忙清除
        @JvmStatic
        fun show(){
            Log.i("lee","java_show: 111")
            Log.i("lee","java_show: 222")
            Log.i("lee","java_show: 333")
            Log.i("lee","java_show: 444")
            Log.i("lee","java_show: 555")
            throw NullPointerException("我是java中的抛出的异常，我的show方法里面发送了Java语法错误")
        }
    }

    private lateinit var binding: ActivitySecondBinding
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivitySecondBinding.inflate(layoutInflater)
        setContentView(binding.root)
        binding.apply {
            btn1.setOnClickListener { exception1() }
            btn2.setOnClickListener {
                try {
                    exception2()
                }catch (e: NoSuchFieldException){
                    e.printStackTrace()
                    Log.i("lee","Java层收到了C++层抛给我的NoSuchFieldException异常！！！")
                }
            }
            btn3.setOnClickListener { exception3() }
        }
    }
}