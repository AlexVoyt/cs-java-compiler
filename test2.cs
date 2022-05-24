class Program
{
    private int TryParse(int first, int second)
    {
        bool Var;
        return first;
    }

    private void ConsoleWriteLine(int argument)
    {
        int Index = 3 + 5;
        TryParse(3, 17*Index + 430);
        return argument + Index + 9*65;
    }

    private int IsNaN(int arg)
    {
        int Index = 0;
        for(Index = 0; Index < 100; Index += 1)
        {
            arg += Index;
            while(arg <= Index*20)
            {
                ConsoleWriteLine(-7);
            }
        }

        return Index;
    }

    private sbyte Test3()
    {
        return 3;
    }

    private sbyte Test2()
    {
        return Test3();
    }

    private sbyte Test1()
    {
        return Test2();
    }

    private void VoidTest()
    {
        return Test2();
    }

    public double ConsoleReadLine()
    {
        return 10;
    }

    public double CalculatorDoOperation(int cleanNum1, int cleanNum2, int op)
    {
        return 10;
    }

    public void Main(int args)
    {
        int endApp = args;
        ConsoleWriteLine(2);
        ConsoleWriteLine(321*4389*args);
        ConsoleWriteLine(321);
        Test2();

        while (endApp)
        {
            double result = 0;

            double numInput1 = ConsoleReadLine();

            double cleanNum1 = 0;
            while (TryParse(numInput1, cleanNum1))
            {
                numInput1 = ConsoleReadLine();
            }

            double numInput2 = ConsoleReadLine();

            double cleanNum2 = 0;
            while (TryParse(numInput2, cleanNum2))
            {
                numInput2 = ConsoleReadLine();
            }

            result = CalculatorDoOperation(cleanNum1, cleanNum2, op);
            if (IsNaN(result))
            {
                ConsoleWriteLine(3);
            }
            else
            {
                ConsoleWriteLine(IsNaN(32));
            }
        }

        return ;
    }
}
