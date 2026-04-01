object FlowTest {
    def main(args: Array[String]): Unit = {
        var x = 7
        var y = 3
        var result = 0
        if (x > 0) {

            if (x % 2 == 0) {
                result = x * 2
            }
            else {

                if (x > 5) {
                    result = x + 10
                } else {
                    result = x - 1
                }
            }

            } 
        else {
                result = -x
        }

        // val category = x match {

        //     case 0 => "zero"

        //     case 1 => 
        //         if (y > 0) "one-positive"
        //         else "one-negative"

        //     case 2 => 
        //         if (y % 2 == 0) {
        //             if (y > 10) "two-big-even"
        //             else "two-small-even"
        //         } else {
        //             "two-odd"
        //         }

        //     case _ => 
        //         if (x > 10) {
        //             if (y > 5) "large-large"
        //             else "large-small"
        //         } else {
        //             if (x < 0) "negative"
        //             else "other"
        //         }
        // }

        // println(category)

        // var i = 0
        // while (i < 10) {
        //     if (i % 2 == 0) {

        //         if (i == 4) {
        //             println("four")
        //         } else if (i == 6) {
        //             println("six")
        //         } else {
        //             println("even")
        //         }

        //     } else {

        //         if (i > 7) {
        //         println("odd big")
        //         } else {
        //         println("odd small")
        //         }

        //     }

        //     i += 1
        // }

        // for (j <- 1 to 5) {
        //     val msg = j match {

        //         case 1 => "one"

        //         case 2 => 
        //             if (j + x > 5) "two big"
        //             else "two small"

        //         case 3 => 
        //             if (x > y) {
        //                 if (x - y > 3) "three far"
        //                 else "three close"
        //             } else {
        //                 "three reverse"
        //             }

        //         case _ => 
        //             if (j % 2 == 0) "other even"
        //             else "other odd"
        //     }
        //     println(msg)
        // }
        // println(finalResult)
    }
}