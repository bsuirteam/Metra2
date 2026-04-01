object TestApp {

  def add(a: Int, b: Int): Int = {
    a + b
  }

  def main(args: Array[String]): Unit = {

    // --- переменные ---
    val x = 10
    var y = 20

    // --- арифметика ---
    val z = x + y * 2 - 5 / 3 % 2

    // --- логика ---
    if (x > 5 && y < 30 || x == y) {
      println("Logic works")
    } else {
      println("Else branch")
    }

    // --- цикл ---
    for (i <- 1 to 5) {
      println(i)
    }

    // --- until ---
    var k = 0
    while (k < 3) {
      k += 1
    }

    // --- коллекции ---
    val list = 1 :: 2 :: 3 :: Nil
    val list2 = list ++ List(4, 5)
    val list3 = 0 +: list :+ 6

    // --- стрелки ---
    val pairs = List(1 -> "one", 2 -> "two")

    // --- match ---
    val result = x match {
      case 1 => "one"
      case _ => "other"
    }

    // --- функции ---
    val sum = add(x, y)
    val nested = math.max(add(1, 2), 3)

    // --- f / s строки ---
    val name = "Scala"
    val greeting = s"Hello, $name"
    val complex = f"Value: ${x + y}%d"

    // --- типы ---
    val anyVal: Any = x
    if (anyVal.isInstanceOf[Int]) {
      val casted = anyVal.asInstanceOf[Int]
      println(casted)
    }

    // --- try/catch ---
    try {
      val risky = 10 / 0
    } catch {
      case e: Exception => println("Error")
    } finally {
      println("Done")
    }

    // --- class ---
    val obj = new MyClass()
    obj.methodCall()

    // --- lambda ---
    val func = (a: Int) => a * 2

    // --- комментарии ---
    /*
      Multi-line comment
      with operators: + - * /
    */

    // однострочный комментарий

  }
}

class MyClass extends AnyRef with Serializable {
  def methodCall(): Unit = {
    println("method called")
  }
}