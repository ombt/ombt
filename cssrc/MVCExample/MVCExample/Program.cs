using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace MVCExample
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            frmCalcView view = new frmCalcView();
            CalculatorModel model = new CalculatorModel();
            CalcController controller = new CalcController(model, view);

            Application.Run(view);
        }
    }
}