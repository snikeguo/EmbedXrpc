using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EmbedXrpcIdlParser
{
    public interface ICppSerializable
    {
        void EmitStruct(ObjectType_TargetType targetStruct, StreamWriter cfilewriter, StreamWriter hfilewriter);
        void EmitSerializeMacro(ObjectType_TargetType targetStruct, StreamWriter writer);
    }
}
