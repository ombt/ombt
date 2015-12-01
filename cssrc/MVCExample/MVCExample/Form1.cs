using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace MVCExample
{


    /// <summary>
    /// All interaction with the calculator's view should go through here.
    /// This View doesn't know about the Controller, except that it could provide methods for registering a Controller's listeners or delegates.
    /// </summary>
    interface ICalcView
    {
        void AddListener( IController controller );

        string Total
        {
            get;
            set;
        }
    };

    /// <summary>
    /// Windows Form that will host our MVC based functionality.
    /// 
    /// </summary>
    public partial class frmCalcView : Form, ICalcView
    {

        IController controller;

        public frmCalcView( )
        {
            InitializeComponent();
            
        }

        /// <summary>
        /// The view needs to interact with the controller to pass the click events
        /// This could be done with delegates instead.
        /// </summary>
        /// <param name="controller"></param>
        public void AddListener( IController controller )
        {
            this.controller = controller;
        }

        private void lbl_Click(object sender, EventArgs e)
        {
            // Get the text out of the label to determine the letter and pass the click info the controller to distribute.
           controller.OnClick((Int32.Parse(((Label)sender).Text)));
        }

        private void lblPlus_Click(object sender, EventArgs e)
        {
           controller.OnAdd();
        }

        #region ICalcView Members

        public string Total
        {
            get
            {
                return textBox1.Text;
            }
            set
            {
                textBox1.Text = value;
            }
        }

        #endregion

      
    }

    /// <summary>
    /// 
    /// </summary>
    public interface IController
    {
        void OnClick(int number);
        void OnAdd();
    }

    /// <summary>
    /// The controller process the user requests.
    /// Based on the user request, the Controller calls methods in the View and Model to accomplish the requested action. 
    /// </summary>
    class CalcController :  IController
    {
        ICalcModel model;
        ICalcView  view;
      

        public CalcController( ICalcModel model, ICalcView view)
        {
            this.model = model;
            this.view  = view;
            this.view.AddListener(this);
        }

        public void OnClick( int number )
        {
            view.Total = model.SetInput(number).ToString();
        }

        public void OnAdd()
        {
            model.ChangeToAddState();
        }

    }

    /// <summary>
    /// Calculator model, The model is independent of the user interface.
    /// It doesn't know if it's being used from a text-based, graphical, or web interface
    /// </summary>
    interface ICalcModel
    {
        /// <summary>
        /// This will process the number supplied and return a result based on it's internal state;
        /// </summary>
        /// <param name="number"></param>
        /// <returns></returns>
        int SetInput(int number);

        /// <summary>
        /// Adds two numbers
        /// </summary>
        /// <param name="value1"></param>
        /// <param name="value2"></param>
        /// <returns></returns>
        int Add(int value1, int value2);

        /// <summary>
        /// Subtracts two numbers
        /// </summary>
        /// <param name="value1"></param>
        /// <param name="value2"></param>
        /// <returns></returns>
        int Subtract(int value1, int value2);
      

        /// <summary>
        /// Change State to Adding
        /// </summary>
        void ChangeToAddState();
      
    };


    /// <summary>
    /// Calculator model, The model is independent of the user interface.
    /// It doesn't know if it's being used from a text-based, graphical, or web interface
    /// This particular model holds the state of the application and the current value.
    /// The current value is updated by SetInput
    /// </summary>
    class CalculatorModel : ICalcModel
    {
        public enum States { NoOperation, Add, Subtract };
        States state;
        int currentValue;

        public States State
        {
            set { state = value; }
        }

        public int SetInput ( int number )
        {
            if (state == States.NoOperation)
            {
                currentValue = number;
                
            }
            else if (state == States.Add)
            {
                currentValue = Add(currentValue , number );
            }

            return currentValue;
        }

        public void ChangeToAddState()
        {
            this.state = States.Add;
        }

        public int Add( int value1, int value2 )
        {
            return value1 + value2;
        }

        public int Subtract(int value1, int value2)
        {
            throw new System.ApplicationException(" Not implemented yet");
        }
    }



}