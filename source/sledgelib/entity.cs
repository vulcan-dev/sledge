using System.Runtime.InteropServices;

namespace SledgeLib
{
    internal class CEntity
    {
        [DllImport("sledge.dll")] internal static extern uint Entity_GetSibling(uint iHandle);
        [DllImport("sledge.dll")] internal static extern uint Entity_GetChild(uint iHandle);
        [DllImport("sledge.dll")] internal static extern uint Entity_GetParent(uint iHandle);
        [DllImport("sledge.dll")] internal static extern void Entity_Destroy(uint iHandle);
        [DllImport("sledge.dll")] internal static extern void Entity_SetParent(uint iHandle, uint iParentHandle);
    }
}