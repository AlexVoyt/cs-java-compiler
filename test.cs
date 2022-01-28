using System;

class Program
{
    private void ConsoleWriteLine()
    {
    }

    public void Main(int args)
    {
        int endApp = 0;
        X = 32;
        enaApp = X;
        endApp = enaApp;
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

            numInput2 = ConsoleReadLine();

            double cleanNum2 = 0;
            while (TryParse(numInput2, cleanNum2))
            {
                numInput2 = ConsoleReadLine();
            }

            result = CalculatorDoOperation(cleanNum1, cleanNum2, op);
            if (IsNaN(result))
            {
                ConsoleWriteLine();
            }
            else
            {
                ConsoleWriteLine();
            }
        }

        return ;
    }
}

