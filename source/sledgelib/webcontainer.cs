using System.Runtime.InteropServices;
using System.Numerics;

namespace SledgeLib
{

    public class CWebContainer
    {
        [DllImport("sledge.dll")] private static extern uint ContainerCreate(uint iWidth, uint iHeight, int iX, int iY);
        [DllImport("sledge.dll")] private static extern void ContainerDestroy(uint iHandle);
        [DllImport("sledge.dll")] private static extern void ContainerSetSize(uint iHandle, Vector2 vPosition);
        [DllImport("sledge.dll")] private static extern void ContainerSetPos(uint iHandle, Vector2 vPosition);
        [DllImport("sledge.dll")] private static extern void ContainerLoadURL(uint iHandle, string sURL);
        [DllImport("sledge.dll")] private static extern void ContainerRunJS(uint iHandle, string sJS);

        internal uint m_Handle = 0;

        private Vector2 _m_Position = new Vector2(0, 0);
        public Vector2 m_Position
        {
            get { return _m_Position; }
            set { _m_Position = value; ContainerSetPos(m_Handle, _m_Position); }
        }

        private Vector2 _m_Size = new Vector2(0,0);
        public Vector2 m_Size
        {
            get { return _m_Size; }
            set { _m_Size = value; ContainerSetSize(m_Handle, _m_Size); }
        }

        public void LoadURL(string sURL) { ContainerLoadURL(m_Handle, sURL); }

        public void RunJS(string sJS) { ContainerRunJS(m_Handle, sJS); }

        public CWebContainer(uint iWidth, uint iHeight, int iX = 0, int iY = 0)
        {
            m_Handle = ContainerCreate(iWidth, iHeight, iX, iY);
            _m_Position = new Vector2(iX, iY);
            _m_Size = new Vector2(iWidth, iHeight);
        }

        ~CWebContainer() { ContainerDestroy(m_Handle); }
    }
}