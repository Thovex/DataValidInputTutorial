using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using UnityEngine;

namespace Thovex
{
    public static class Utility
    {
        public static void For3(Vector3Int data, Action<int, int, int> iterator)
        {
            for (int x = 0; x < data.x; x++)
            {
                for (int y = 0; y < data.y; y++)
                {
                    for (int z = 0; z < data.z; z++)
                    {
                        iterator(x, y, z);
                    }
                }
            }
        }

        public static void For3(Vector3Int data, int iteratorJumpSize, Action<int, int, int> iterator)
        {
            for (int x = 0; x < data.x; x += iteratorJumpSize)
            {
                for (int y = 0; y < data.y; y += iteratorJumpSize)
                {
                    for (int z = 0; z < data.z; z += iteratorJumpSize)
                    {
                        iterator(x, y, z);
                    }
                }
            }
        }
    }
}