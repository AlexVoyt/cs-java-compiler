class Program
{
    private void ConsoleWriteLine(int argument)
    {
        return argument;
    }

    private int TryParse(int first, int second)
    {
        return first ;
    }

    private int IsNaN(int arg)
    {
        for(int Index = 0; Index < 100; ++Index)
        {
            arg += Index;
        }

        return Index;
    }

    public double ConsoleReadLine()
    {
        return 10;
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
        ConsoleWriteLine(321);

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
