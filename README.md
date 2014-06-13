1. Overview
===========

```
.
├── binder_test         binder驱动测试程序，发送两个数字，service做加法返回。
├── bw_binder           测试binder的bandwidth
├── bw_pipe             测试pipe的bandwidth
├── bw_tcp              测试tcp的bandwidth
├── exploit             以下两个为service的权限提升poc，可忽略
├── exploit-normal      同上
├── lat2_binder         测试binder的latency
├── lat_binder          测试binder的latency
├── lat_pipe            测试pipe的latency
├── lat_tcp             测试tcp的latency
├── lib                 以上bandwidth和latency测试用到的公共函数，编译成so
└── staps               systemtap脚本
└── module-centos       binder新旧驱动，在centos6.5上作为模块编译
└── tools               get-diff.sh用来对比内核版本
└── docs                相关文档
└── README
└── run_scripts         循环执行测试程序，获得bandwidth和latency与数据量以及
                        concurrents的关系.
```

2. How to run
============

- Put project in external/ of android source code.
- Compile Android source,then it will be compiled into bins.or you can 
  `mmm external/your/path`.
- Push the `bw_*` and `lat_*` in android smartphone.
- Run the bins through adb on android.

3. Synopsis
===========

If you don't know how to run it,please run it with `--help`.
e.g.: 

```
./bw_binder --help
-m: send msg size each rpc
-M: send msg size each iteration
-n: iteration num.
```

