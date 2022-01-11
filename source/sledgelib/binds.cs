using System.Runtime.InteropServices;

namespace SledgeLib
{
    public delegate void dBindCallback();
    public delegate void dAdvancedBindCallback(bool bKeyDown);
    public delegate void dInputReader(EKeyCode iKeyCode, bool bKeyDown);

    internal class CBindManager
    {
        [DllImport("sledge.dll")] public static extern void RegisterInputReader(dInputReader InputReader);

        internal static List<CBind> m_Binds = new List<CBind>();

        internal static void OnInput(EKeyCode iKeyCode, bool bKeyDown)
        {
            foreach (CBind Bind in m_Binds.ToList())
            {
                if (!Bind.m_Active)
                    continue;

                if (iKeyCode != Bind.m_KeyCode)
                    continue;

                if (Bind.m_Callback != null)
                {
                    if (!bKeyDown)
                        continue;

                    try
                    {
                        Bind.m_Callback();
                    }
                    catch (Exception ex)
                    {
                        Log.Error("Error while executing bind: {0}", ex.Message);
                        lock (m_Binds) { m_Binds.Remove(Bind); }
                    }
                    continue;
                }

                if (Bind.m_AdvancedCallback != null)
                {
                    try
                    {
                        Bind.m_AdvancedCallback(bKeyDown);
                    }
                    catch (Exception ex)
                    {
                        Log.Error("Error while executing bind: {0}", ex.Message);
                        lock (m_Binds) { m_Binds.Remove(Bind); }
                    }
                    continue;
                }
            }
        }
        internal static dInputReader InputReader = new dInputReader(OnInput);
    }

    public class CBind
    {
        internal EKeyCode m_KeyCode;

        internal dBindCallback? m_Callback;
        internal dAdvancedBindCallback? m_AdvancedCallback;

        public bool m_Active = false;

        public CBind(EKeyCode eKey, dBindCallback Callback, bool bActive = true)
        {
            m_KeyCode = eKey;
            m_Callback = Callback;
            m_Active = bActive;

            lock (CBindManager.m_Binds) { CBindManager.m_Binds.Add(this); }
        }

        public CBind(EKeyCode eKey, dAdvancedBindCallback Callback, bool bActive = true)
        {
            m_KeyCode = eKey;
            m_AdvancedCallback = Callback;
            m_Active = bActive;

            lock (CBindManager.m_Binds) { CBindManager.m_Binds.Add(this); }
        }

        public void Unregister()
        {
            lock (CBindManager.m_Binds) { CBindManager.m_Binds.Remove(this); }
        }
    }
}
