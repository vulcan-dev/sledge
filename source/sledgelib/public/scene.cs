using System.Numerics;
using System.Runtime.InteropServices;

namespace SledgeLib
{
    public class Scene
    {
        [DllImport("sledge_core.dll")] internal static extern QueryInfo _QueryRaycast(Vector3 Origin, Vector3 Direction, float MaxDist);
        [DllImport("sledge_core.dll")] internal static extern QueryInfo _QuerySpherecast(Vector3 Origin, Vector3 Direction, float Radius, float MaxDist);
        [DllImport("sledge_core.dll")] internal static extern QueryInfo _QueryClosestPoint(Vector3 Origin, float MaxDist);
        [DllImport("sledge_core.dll")] internal static extern uint _MakeHole(Vector3 Position, float SoftRadius, float MidRadius, float HardRadius, bool Silent);
        [DllImport("sledge_core.dll")] internal static extern void _DrawLine(Vector3 Origin, Vector3 Goal, Vector4 Colour, bool UseDepth);
        [DllImport("sledge_core.dll")] internal static extern void _DrawCross(Vector3 Position, Vector4 Colour, float Size);
        [DllImport("sledge_core.dll")] internal static extern void _Paint(Vector3 Position, EPaintType Type, float Radius, float Probability);

        public static QueryInfo QueryRaycast(Vector3 Origin, Vector3 Direction, float MaxDist)
        {
            if (Game.State != EGameState.Playing)
                return new QueryInfo();

            return _QueryRaycast(Origin, Direction, MaxDist);
        }

        public static QueryInfo QuerySpherecast(Vector3 Origin, Vector3 Direction, float Radius, float MaxDist)
        {
            if (Game.State != EGameState.Playing)
                return new QueryInfo();

            return _QuerySpherecast(Origin, Direction, Radius, MaxDist);
        }

        public static QueryInfo QueryClosestPoint(Vector3 Origin, float MaxDist)
        {
            if (Game.State != EGameState.Playing)
                return new QueryInfo();

            return _QueryClosestPoint(Origin, MaxDist);
        }

        public static uint MakeHole(Vector3 Position, float SoftRadius, float MidRadius, float HardRadius, bool Silent)
        {
            if (Game.State != EGameState.Playing)
                return 0;

            return _MakeHole(Position, SoftRadius, MidRadius, HardRadius, Silent);
        }

        public static void DrawLine(Vector3 Origin, Vector3 Goal, Vector4 Colour, bool UseDepth)
        {
            if (Game.State != EGameState.Playing)
                return;

            _DrawLine(Origin, Goal, Colour, UseDepth);
        }

        public static void DrawCross(Vector3 Position, Vector4 Colour, float Size)
        {
            if (Game.State != EGameState.Playing)
                return;

            _DrawCross(Position, Colour, Size);
        }

        public static void Paint(Vector3 Position, EPaintType Type, float Radius, float Probability)
        {
            if (Game.State != EGameState.Playing)
                return;

            _Paint(Position, Type, Radius, Probability);
        }
    }
}