
#include "JsonUtil.h"
#include <sstream>

using namespace ThorsAnvil::Json;

/* Forward declaration of static functions */
static void mergeJsonDom(JsonMap&   dst, JsonMap&   src, std::string const& errorMsg, std::string const& index);
static void mergeJsonDom(JsonArray& dst, JsonArray& src, std::string const& errorMsg, std::string const& index);


/* Implementation */
static void mergeJsonDom(JsonMap& dst, JsonMap& src, std::string const& errorMsg, std::string const& index)
{
    for(JsonMap::iterator loop = src.begin(); loop != src.end(); ++loop)
    {
        JsonMap::iterator find = dst.find(loop->first);
        if (find == dst.end())
        {
            dst.transfer(loop, src);
        }
        else
        {
            JsonNULLItem*   nil = dynamic_cast<JsonNULLItem*>(loop->second);
            if (nil != NULL)
            {
                // JsonNull objects cause the node in the destination to be deleted.
                dst.erase(find);
                continue;
            }

            JsonMapItem*    map = dynamic_cast<JsonMapItem*>(find->second);
            JsonArrayItem*  arr = dynamic_cast<JsonArrayItem*>(find->second);


            if ((map == NULL) && (arr == NULL))
            {
                // Normal Item (not map or array). 
                // The src item over-rides the destination item
                dst.erase(find);
                dst.transfer(loop, src);
            }
            else
            {
                // The item in the destination is a map or an array.
                // Check what the value we are merging into this object is
                JsonMapItem*    srcMapItem = dynamic_cast<JsonMapItem*>(loop->second);

                if ((map != NULL) && (srcMapItem == NULL))
                {
                    // Maps can only be combined with other maps.

                    char const* into = (map != NULL)        ? "Map" : "Array";
                    char const* from = (srcMapItem != NULL) ? "Map" : "Other";
                    std::stringstream msg;
                    msg << "Invalid Config Merge into " << into << " from " << from << " in file: `" << errorMsg << "` into `" << index << "`:" << find->first;
                    throw std::runtime_error(msg.str());
                }

                if (map != NULL) // If he destination is a map then the source must be a map
                {
                    // Recursively combine maps.
                    JsonMap&  dstMap    = *map->value;  
                    JsonMap&  srcMap    = *srcMapItem->value;

                    mergeJsonDom(dstMap, srcMap, errorMsg, index + ":" + find->first);
                }
                else if (arr != NULL)
                {
                    // Combine stuff into an array.
                    JsonArrayItem*    srcArrayItem = dynamic_cast<JsonArrayItem*>(loop->second);

                    if (srcArrayItem != NULL)
                    {
                        // Two arrays are concatenate together.
                        JsonArray&  dstArray    = *arr->value;  
                        JsonArray&  srcArray    = *srcArrayItem->value;

                        mergeJsonDom(dstArray, srcArray, errorMsg, index);
                    }
                    else
                    {
                        // Normal Items are just append onto the end of the array
                        JsonArray&  dstArray    = *arr->value;  
                        dstArray.push_back(src.release(loop).release());
                    }
                }
            }
        }
    }
}

static void mergeJsonDom(JsonArray& dst, JsonArray& src, std::string const& /*errorMsg*/, std::string const& /*index*/)
{
    dst.transfer(dst.end(), src.begin(), src.end(), src);
}

void ThorsAnvil::Json::mergeJsonDom(JsonMap& dst, JsonMap& src, std::string const& errorMsg)
{
    ::mergeJsonDom(dst, src, errorMsg, "root");
}

void ThorsAnvil::Json::mergeJsonDom(JsonArray& dst, JsonArray& src, std::string const& errorMsg)
{
    ::mergeJsonDom(dst, src, errorMsg, "root");
}



