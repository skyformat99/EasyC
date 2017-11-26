# EasyC
`EasyC`提供了丰富的API用于简化Windows环境下C语言编程。

Windows环境下的C语言编程存在非常多的困难:
* 缺乏类似C++, Java, Python等语言所具备的容器和迭代器功能。没有字典, 可变数组, 字符串, LRU缓存, 链表等数据结构。在面临相关业务时要求程序员自己实现，增加编程难度。
* 缺乏基本的算法，没有直接提供字符串查询，排序，交集并集等功能。这同样增加了编程门槛。
* 缺乏更为方便的Windows API编程接口。若要使用Windows系统级服务，需要深入学习Windows底层原理，并且Windows API接口设计的并不算友好，直接采用原生API进行开发复杂度较高。

介于此，该工具聚合了`容器`，`算法`和`系统`等三大块的功能，全面解决了上述问题，并采用面向对象的方式组织API，直观清晰，可以更好的维护程序代码。

## 目录
- [安装与使用](doc/install.md)
- [API](doc/api.md)
	- [容器API](doc/api.md)
		- [Vector](doc/api.md)
		- [List](doc/api.md)
		- [Matrix](doc/api.md)
		- [String](doc/api.md)
		- [Stack](doc/api.md)
		- [Queue](doc/api.md)
		- [HashMap](doc/api.md)
		- [TreeMap](doc/api.md)
		- [LRU](doc/api.md)
	- [算法API](doc/api.md)
		- [sort](doc/api.md)
		- [minInVector/maxInVector](doc/api.md)
		- [STC](doc/api.md)
		- [KMP](doc/api.md)
	- [系统API](doc/api.md)
		- [RIO](doc/api.md)
		- [Process](doc/api.md)
		- [funWrapper](doc/api.md)
- [原理](doc/theory.md)
	- [容器原理](doc/theory.md)
		- [Vector](doc/theory.md)
		- [List](doc/theory.md)
		- [Matrix](doc/theory.md)
		- [String](doc/theory.md)
		- [Stack](doc/theory.md)
		- [Queue](doc/theory.md)
		- [HashMap](doc/theory.md)
		- [TreeMap](doc/theory.md)
		- [LRU](doc/theory.md)
	- [算法原理](doc/theory.md)
		- [sort](doc/theory.md)
		- [minInVector/maxInVector](doc/theory.md)
		- [STC](doc/theory.md)
		- [KMP](doc/theory.md)
	- [系统原理](doc/theory.md)
		- [RIO](doc/theory.md)
		- [Process](doc/theory.md)
		- [funWrapper](doc/theory.md)
- [致谢](doc/thanks.md)