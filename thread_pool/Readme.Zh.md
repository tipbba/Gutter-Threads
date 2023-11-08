# 关于Gutter所需要知道的一切

**Gutter**是作者在阅读```《C++并发编程实战》```一书，读书过程中进行练习的产物。在一开始，**Gutter**的代码只是原书第9章的简单复刻，随着不断的学习，**Gutter**的代码也随之发生了一些变化。但整体而言，**Gutter**依然保持了功能简单，代码易读的特点，比较适合作为一个学习多线程编程思想的项目。这也正是**Gutter**名称的由来 —— 在```C++```广袤的知识海洋面前，**Gutter**只是一个小水沟。

**Gutter**的特点：
- **Gutter**基于```Linux Kernel 6.2``` 与 ```GCC 11.3.0```环境开发。

- **Gutter**拥有了双层任务队列：为减少不同线程不必要的资源竞争，**Gutter**拥有双层的任务队列，即全局的任务队列以及每个线程专有任务队列。

- **Gutter**支持任务“窃取”功能：当出现一部分线程疲于奔命，而其他线程却处于闲置状态时，**Gutter**可以令闲置的线程自动窃取忙碌线程的任务，从而提高处理效率。

**Gutter**的未来:

在未来，作者将在保持整体简洁易懂的状态下，继续优化**Gutter**的功能。此外，如果精力允许，作者可能会在```Readme```文档中，更新代码的详解 —— 尽管可能根本没人看（狗头）。

```
//目前有一个重大bug，即书中队列使用mutex和条件变量控制线程，当初始化时，所有队列均为空，配合yield()实现令所有线程休眠，当任务进入队列，使用
//wait.notify_all()唤醒池中线程，但我实现的自旋锁缺少该特点，各线程不会休眠，不断自旋使线程池创建过程出现死锁，因此，可以考虑使用
//condition_variable_any类实现wait方法，使无任务的线程进入休眠状态！

//在执行上述计划之前，应考虑使用gbd进行多线程调试，从而确定问题的所在！

```