package com.sawyer.studyjniexception

import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import com.sawyer.studyjniexception.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    companion object {
        init {
            System.loadLibrary("studyjniexception")
        }
        // 假设这里定义了一大堆静态变量
        @JvmStatic
        var name1 = "T1"
        @JvmStatic
        var name2 = "T2"
        @JvmStatic
        var name3 = "T3"
        @JvmStatic
        external fun localCache(name: String) //普通局部缓存示例
        //使用静态缓存
        @JvmStatic
        external fun initStaticCache() //初始化静态缓存
        @JvmStatic
        external fun staticCache(name: String) //静态缓存
        @JvmStatic
        external fun clearStaticCache() //清除静态缓存
    }

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        binding.apply {
            btn1.setOnClickListener {
                val intArray = intArrayOf(1,-2,34,3,88,34,0)
                sort(intArray)
                for (i in intArray){
                    Log.i("lee", "$i \t")
                }
            }
            btn2.setOnClickListener {
                localCache(name1)
                localCache(name2)
                localCache(name3)
            }
            btn3.setOnClickListener {
                staticCacheAction()
            }
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        clearStaticCache()
    }

    external fun stringFromJNI(): String
    private external fun sort(arr: IntArray)//数组排序

    private fun staticCacheAction(){
        //1.先初始化静态缓存(注意：如果是一个类去调用，就需要在构造函数中初始化)
        initStaticCache()
        //2.执行静态缓存
        staticCache("李白")
        Log.i("lee", "静态缓存区域name1 = $name1")
        Log.i("lee", "静态缓存区域name2 = $name2")
        Log.i("lee", "静态缓存区域name3 = $name3")
        staticCache("杜甫")
        Log.i("lee", "静态缓存区域name1 = $name1")
        Log.i("lee", "静态缓存区域name2 = $name2")
        Log.i("lee", "静态缓存区域name3 = $name3")
        staticCache("屈原")
        Log.i("lee", "静态缓存区域name1 = $name1")
        Log.i("lee", "静态缓存区域name2 = $name2")
        Log.i("lee", "静态缓存区域name3 = $name3")
    }

}