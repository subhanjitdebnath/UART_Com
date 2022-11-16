using System;
using System.Threading;
using System.IO.Ports;

namespace FrequencySimulator
{
    public class UARTCom
    {
        SerialPort serialPort;
        String ReceivedString;
        public UARTCom()
        { }
        public void Connect(string Port, int Baudrate)
        {
            serialPort = new SerialPort();
            serialPort.PortName = Port;
            serialPort.BaudRate = Baudrate;
            serialPort.Parity = Parity.None;
            serialPort.StopBits = StopBits.One;
            // Set the read/write timeouts  
            serialPort.ReadTimeout = 2000;
            serialPort.WriteTimeout = 2000;
            serialPort.Open();
        }
        private void ReadString()
        {
            ReceivedString = serialPort.ReadExisting();
        }
        public void WriteData(string msg)
        {
            serialPort.Write(msg + "\n");
        }
        public void Frequency_Duty()
        {
            Thread.Sleep(1000);
            ReadString();
            if (ReceivedString.Contains("F=") && ReceivedString.Contains("D="))
            {
                Console.WriteLine("Frequency=" + ReceivedString.Split('$')[1].ToString() + " Hz");
                Console.WriteLine("Duty=" + ReceivedString.Split('$')[3].ToString() + " %");
            }



        }
    }
}
