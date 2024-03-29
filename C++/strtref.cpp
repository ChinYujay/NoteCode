﻿//程序清单8.6 strtref.cpp
//strc ref.cpp -- using structure references
#include <iostream>
#include <string>
struct free_throws
std::string name;
{
	int made;
	int attempts;
	float percent;
};

void display(const free_throws & ft);
void set_pc(free_throws & ft);
free_throws & accumulate(free_throws & target, const free_throws & source);

int main()
{
// partial initializations - remaining members set to 0
	free_throws one ={"Ifelsa Branch", 13, 14};
	free throws two ={"Andor Knott", 10, 16};
	free_throws three ={"Minnie Max", 7, 9};
	free_throws four ={"Whily Looper", 5, 9};
	free throws five = {"Long Long", 6, 14};
	free throws team ={"Throwgoods", 0, 0};
// no initialization
	free_throws dup;

	set_pc(one);
	display(one);
	accumulate(team, one);
	display(team);
// use return value as argument
	display(accumulate(team, two));
	accumulate(accumulate(team, three),four);
	display(team);
// use return value in assignment
	dup = accumulate(team,five);
	std::cout << "Displaying team:\n";
	display(team);
	std::cout << "Displaying dup after assignment:\n";
	display(dup) ;
	set_pc(four);
// ill-advised assignment
	accumulate(dup,five) = four;
	std::cout << "Displaying dup after ill-advised assignment:\n";
	display(dup);
	return 0;
}

void display(const free_throws & ft)
{
	using std::cout;
	cout << "Name:" << ft.name << '\n';
	cout << " Made: "<< ft.made << '\t';
	cout << "Attempts: " << ft.attempts << '\t';
	cout << "Percent: " << ft.percent << '\n';
}
void set_pc(free_throws & ft)
{
	if (ft.attempts != 0)
		ft.percent = 100.0f *float(ft.made)/float(ft.attempts);
	else
		ft.percent = 0;
}

free_throws & accumulate(free_throws & target, const free_throws & source)
{
	target.attempts += source.attempts;
	target.made += source.made;
	set_pc(target);
	return target;
}
/*运行结果如下：
Name:Ifelsa Branch
 Made: 13       Attempts: 14    Percent: 92.8571
Name:Throwgoods
 Made: 13       Attempts: 14    Percent: 92.8571
Name:Throwgoods
 Made: 23       Attempts: 30    Percent: 76.6667
Name:Throwgoods
 Made: 35       Attempts: 48    Percent: 72.9167
Displaying team:
Name:Throwgoods
 Made: 41       Attempts: 62    Percent: 66.129
Displaying dup after assignment:
Name:Throwgoods
 Made: 41       Attempts: 62    Percent: 66.129
Displaying dup after ill-advised assignment:
Name:Whily Looper
 Made: 5        Attempts: 9     Percent: 55.5556
*/
/*
1.程序说明
该程序首先初始化了多个结构对象。如果指定的初始值比成员少，
余下的成员（这里只有percent）将被设置为零。第一个函数调用如下：
set_pc(one);
由于函数set_pc()的形参ft为引用，因此ft指向one,
函数set_pc()的代码设置成员one.percent。就这里而言，按值传递不可行，
因此这将导致设置的是one的临时拷贝的成员percent。
另一种方法是使用指针参数并传递地址，但要复杂些：
	set_pcp(&one);// using pointers instead - &one instead of one
……
void set_pcp(free_throws * pt)
{
	if (pt->attempts != 0)
		pt->percent = 100.0f *float (pt->made)/float (pt->attempts);
	else
		pt->percent = 0;
}
下一个函数调用如下：
	display(one);

由于display()显示结构的内容，而不修改它，因此这个函数使用了一个const引用参数。
就这个函数而言，也可按值传递结构，但与复制原始结构的拷贝相比，使用引用可节省时间和内存。
再下一个函数调用如下：
accumulate(team, one);

函数accumulate()接收两个结构参数，并将第二个结构的成员attempts和made的数据添加到第一个结构的相应成员中。
只修改了第一个结构，因此第一个参数为引用，而第二个参数为const引用：
	free_throws & accumulate(free_throws & target, const free_throws & source);

返回值呢？当前讨论的函数调用没有使用它；就目前而言，原本可以将返回值声明为void，但请看下述函数调用：
	display(accumulate(team, two));

上述代码是什么意思呢？首先，将结构对象team作为第一个参数传递给了accumulate()。
这意味着在函数accumulate()中，target指向的是team。函数accumulate()修改team,再返回指向它的引用。
注意到返回语句如下：
	return target;

光看这条语句并不能知道返回的是引用，但函数头和原型指出了这一点：
	free_throws & accumulate(free_throws & target, const free_throws & source)

如果返回类型被声明为free_throws 而不是free_throws &,上述返回语句将返回target(也就是 team)的拷贝。
但返回类型为引用，这意味着返回的是最初传递给accumulate()的team对象。
接下来，将accumulate()的返回值作为参数传递给了display(),这意味着将team传递给了display()。
display()的参数为引用，这意味着函数 display()中的ft指向的是team,因此将显示team的内容。
所以，下述代码：
	display(accumulate (team, two));
	
与下面的代码等效：
	accumulate(team, two);
	display(team);

上述逻辑也适用于如下语句：
	accumulate(accumulate (team, three), four);
	
因此，该语句与下面的语句等效：
	accumulate(team, three);
	accumulate(team, four);
	
接下来，程序使用了一条赋值语句：
	dup = accumulate(team, five) ;
	
正如您预期的，这条语句将team中的值复制到dup中。
最后，程序以独特的方式使用了accumulate()：
	accumulate(dup,five) = four;
这条语句将值赋给函数调用，这是可行的，因为函数的返回值是一个引用。
如果函数accumulate()按值返回，这条语句将不能通过编译。
由于返回的是指向dup的引用，因此上述代码与下面的代码等效：
	accumulate(dup,five); // add five's data to dup
	dup = four;				// overwrite the contents of dup with the contents of four
其中第二条语句消除了第一条语句所做的工作，因此在原始赋值语句使用accumulate()的方式并不好。

2.为何要返回引用
下面更深入地讨论返回引用与传统返回机制的不同之处。传统返回机制与按值传递函数参数类似：
计算关键字returm后面的表达式，并将结果返回给调用函数。
从概念上说，这个值被复制到一个临时位置,
而调用程序将使用这个值。请看下面的代码：
	double m = sqrt(16.0);
	cout << sqrt (25.0);
在第一条语句中，值4.0被复制到一个临时位置，然后被复制给m。
在第二条语句中，值5.0被复制到一个临时位置，然后被传递给cout
(这里理论上的描述，实际上，编译器可能合并某些步骤).
现在来看下面的语句：
	dup = accumulate(team, five);
如果accumulate()返回一个结构，而不是指向结构的引用，将把整个结构复制到一个临时位置，
再将这个拷贝复制给dup。但在返回值为引用时，将直接把team复制到dup，其效率更高。
注意：返回引用的函数实际上是被引用的变量的别名。

3.返回引用时需要注意的问题
返回引用时最重要的一点是，应避免返回函数终止时不再存在的内存单元引用。
您应避免编写下面这样的代码：
	const free_throws & clone2(free_throws & ft)
	free_throws newguy; // first step to big error
	newguy = ft;
// copy info
// return reference to copy
	return newguy;
该函数返回一个指向临时变量(newguy)的引用，函数运行完毕后它将不再存在。
同样，也应避免返回指向临时变量的指针。为避免这种问题，最简单的方法是，
返回一个作为参数传递给函数的引用。作为参数的引用将指向调用函数使用的数据，
因此返回的引用也指向这些数据。
另一种方法是用new来分配新的储存空间。并返回指向该内存空间的指针。
下面是使用引用来完成类似工作的方法：
const free_throws & clone(free throws & ft)
{
	free_throws * pt;
	*pt = ft;	// copy info
	return *pt;	// return reference to copy
}
第一条语句创建一个无名的 free_throws 结构，并让指针pt 指向该结构，因此*pt就是该结构。
上述代码似乎会返回该结构，但函数声明表明，该函数实际上将返回这个结构的引用。
这样，便可以这样使用该函数：
	free_throws & jolly = clone(three);
这使得jolly成为新结构的引用。这种方法存在一个问题：在不再需要new分配的内存时，应使用delete
来释放它们。调用clone(）隐藏了对new的调用，这使得以后很容易忘记使用delete 来释放内存。

4.为何将const用于引用返回类型
如下语句：
	accumulate(dup,five) = four;
其效果如下：首先将five的数据添加到dup中,再使用four的内容覆盖dup的内容。
这条语句为何能够通过编译呢？在赋值语句中，左边必须是可修改的左值。
也就是说，在赋值表达式中，左边的子表达式必须标识一个可修改的内存块。
在这里，函数返回指向dup的引用，它确实标识的是一个这样的内存块，因此这条语句是合法的。

另一方面，常规（非引用）返回类型是右值——不能通过地址访问的值。
这种表达式可出现在赋值语句的右边，但不能出现在左边。其他右值包括字面值(如10.0)和表达式(如x+y)。
显然，获取字面值(如10.0)的地址没有意义，但为何常规函数返回值是右值呢？

这是因为这种返回值位于临时内存单元中，运行到下一条语句时，它们可能不再存在。
假设您要使用引用返回值，但又不允许执行像给accumulate()赋值这样的操作，只需将返回类型声明为const引用：

	const free_throws & accumulate(free_throws & target, const free_throws & source);

现在返回类型为const，是不可修改的左值，因此下面的赋值语句不合法：
accumulate(dup,five) = four; // not allowed for const reference return
该程序中的其他函数调用又如何呢？返回类型为const引用后，下面的语句仍合法：
display (accumulate (team, two)) ;
这是因为display()的形参也是const free_throws &类型。
但下面的语句不合法，因此accumulate()的第一个形参不是const:

	accumulate(accumulate(team, three),four);
这影响大吗？就这里而言不大，因为您仍可以这样做：
	accumulate(team, three);
	accumulate(team, four);
另外，您仍可以在赋值语句右边使用accumulate()。
通过省略const，可以编写更简短代码，但其含义也更模糊。
通常，应避免在设计中添加模糊的特性，因为模糊特性增加了犯错的机会。将返回类型声明为const引用，
可避免您犯糊涂。然而，有时候省略const确实有道理,<<就是一个这样的例子。
*/

/*笔记：
 *1.函数的函数头和原型可以指出函数的返回值类型是否为引用，但是返回(return)不会看出
 *2.返回类型为引用，并且形式参数为引用，意味着返回的是最初传递给函数的实际参数
 *3.如果函数返回一个变量，而不是指向变量的引用，将把整个变量复制到一个临时位置，
 *  再将这个拷贝复制给使用该变量的函数。
 *  但在返回值为引用时，将直接把原来的变量复制到调用函数，其效率更高。
 *4.返回引用的函数实际上是被引用的变量的别名。
 *
 */