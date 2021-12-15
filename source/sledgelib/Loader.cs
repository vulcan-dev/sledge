namespace SledgeLib
{

    struct CSledgeAPI_Internal
    {
        public delegate void WriteLogDelegate(string cMsg);
        public WriteLogDelegate WriteLog;
    }

    public delegate IntPtr GetInternalDelegate();

    public delegate void InitDelegate(GetInternalDelegate GetInternal);

    public class Loader
    {
        public static void Init(GetInternalDelegate GetInternal)
        {
            Console.WriteLine("C# init called");

            IntPtr pInternalAPI = GetInternal();
            if (pInternalAPI == IntPtr.Zero)
                return;

            CSledgeAPI_Internal pAPI = (CSledgeAPI_Internal)System.Runtime.InteropServices.Marshal.PtrToStructure(pInternalAPI, typeof(CSledgeAPI_Internal));

            Console.WriteLine("called GetInternal");

            pAPI.WriteLog("hello world");

            //CSledgeAPI SledgeAPI = (CSledgeAPI)System.Runtime.InteropServices.Marshal.PtrToStructure(pSledgeAPI, typeof(CSledgeAPI));
            //SledgeAPI.WriteLog("hello");

            //CSledgeAPI SledgeAPI = (CSledgeAPI)pSledgeAPI;
            //SledgeAPI.WriteLog("hello");
            //CSledgeAPI
        }
    }
}