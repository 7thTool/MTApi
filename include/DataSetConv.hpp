#pragma once
#include <DataSet.hpp>
#include <string>
#include <sstream>
#include <functional>

namespace MTP {

class DataSetConv
{
protected:
    using conv_handler = std::function<std::string(size_t, void *)>;
    conv_handler conv_;

public:
    DataSetConv()
    {
    }
    DataSetConv(conv_handler conv) : conv_(conv)
    {
    }

    template<typename Target = double>
    inline Target to_number(IDataSet *dataset, size_t id, const Target& def = Target())
    {
        return to_number(dataset, id, 0, def);
    }
    template<typename Target = double>
    inline Target to_number(IDataSet *dataset, size_t id, size_t offset, const Target& def = Target())
    {
        if(!dataset) {
            return def;
        }
        FieldInfo info = {0};
        void* ptr = (void*)dataset->GetFieldValueEx(id, offset, &info);
        if(ptr) {
            switch(info.type)
            {
            case FIELD_TYPE_INT8:
            {
                return (Target)*(uint8_t *)ptr;
            }
            break;
            case FIELD_TYPE_INT16:
            {
                 return (Target)*(uint16_t *)ptr;
            }
            break;
            case FIELD_TYPE_INT32:
            {
                 return (Target)*(uint32_t *)ptr;
            }
            break;
            case FIELD_TYPE_INT64:
            {
                 return (Target)*(int64_t *)ptr;
            }
            break;
            case FIELD_TYPE_DOUBLE:
            {
                 return (Target)*(double *)ptr;
            }
            break;
            case FIELD_TYPE_STRING:
            {
                return (Target)std::stod((const char*)ptr);
            }
            break;
            case FIELD_TYPE_DATASET:
            {

            }
            break;
            default:
            {
                return (Target)std::stod(conv_(info.type, ptr));
            }
            break;
            }
        }
        return def; 
    }

    inline std::string to_string(IDataSet *dataset, size_t id, const std::string& def = std::string())
    {
        return to_string(dataset, id, 0, def);
    }
    inline std::string to_string(IDataSet *dataset, size_t id, size_t offset, const std::string& def = std::string())
    {
        if(!dataset) {
            return def;
        }
        FieldInfo info = {0};
        void* ptr = (void*)dataset->GetFieldValueEx(id, offset, &info);
        if(ptr) {
            switch(info.type)
            {
            case FIELD_TYPE_INT8:
            {
                return std::to_string(*(uint8_t *)ptr);
            }
            break;
            case FIELD_TYPE_INT16:
            {
                 return std::to_string(*(uint16_t *)ptr);
            }
            break;
            case FIELD_TYPE_INT32:
            {
                 return std::to_string(*(uint32_t *)ptr);
            }
            break;
            case FIELD_TYPE_INT64:
            {
                 return std::to_string(*(int64_t *)ptr);
            }
            break;
            case FIELD_TYPE_DOUBLE:
            {
                 return std::to_string(*(double *)ptr);
            }
            break;
            case FIELD_TYPE_STRING:
            {
                return (const char*)ptr;
            }
            break;
            case FIELD_TYPE_DATASET:
            {

            }
            break;
            default:
            {
                return conv_(info.type, ptr);
            }
            break;
            }
        }
        return def; 
    }
    
    std::string to_json(IDataSet *dataset)
    {
        std::stringstream ss;
        ss << R"({)";
        {
			bool first_comma = true;
            size_t i = 0, j = 0;
			size_t field_num = dataset->GetFieldCount();
			FieldInfo* field_info = (FieldInfo*)alloca(sizeof(FieldInfo)*field_num);
            field_num = dataset->GetFieldInfo(field_info, field_num);
            
            first_comma = true;
            i = 0, j = 0;
            ss << R"("fields":[)";
            for (; i < field_num; i++)
            {
				if (!first_comma)
				{
					ss << R"(,)";
				}
				else
				{
					first_comma = false;
				}
                ss << R"({)";
				ss << R"("id":)" << field_info[i].id << R"(,"type":)" << field_info[i].type;
                ss << R"(})";
            }
            ss << R"(])";

            ss << R"(,)";
            first_comma = true;
            i = 0, j = 0;
            for (; i < field_num; i++)
            {
                size_t field_size = dataset->GetFieldSize(field_info[i].id);
                if (field_size == 0)
                {
                    continue;
                }

				if (!first_comma)
				{
					ss << R"(,)";
				}
				else
				{
					first_comma = false;
				}
				ss << R"(")" << field_info[i].id << R"(":)";
                if (field_size == 1)
                {
					void *val = dataset->GetFieldValue(field_info[i].id);
					if (val) {
                        switch (field_info[i].type)
                        {
                        case FIELD_TYPE_INT8:
                        {
                            ss << (int)*(char *)val;
                        }
                        break;
                        case FIELD_TYPE_INT16:
                        {
                            ss << *(int16_t *)val;
                        }
                        break;
                        case FIELD_TYPE_INT32:
                        {
                            ss << *(int32_t *)val;
                        }
                        break;
                        case FIELD_TYPE_INT64:
                        {
                            ss << *(int64_t *)val;
                        }
                        break;
                        case FIELD_TYPE_DOUBLE:
                        {
                            ss << *(double *)val;
                        }
                        break;
                        case FIELD_TYPE_STRING:
                        {
                            ss << R"(")" << (const char *)val << R"(")";
                        }
                        break;
                        case FIELD_TYPE_DATASET:
                        {
                            ss << to_json((IDataSet *)val);
                        }
                        break;
                        default:
                        {
                            if (conv_)
                            {
                                ss << conv_(field_info[i].type, val);
                            }
                            else
                            {
                                ss << R"(")" << val << R"(")";
                            }
                        }
                        break;
                        }
					}
                }
                else
                {
                    ss << R"([)";
                    for (j = 0; j < field_size; j++)
                    {
                        if (j != 0)
                        {
                            ss << R"(,)";
                        }
                        void *val = dataset->GetFieldValue(field_info[i].id, j);
						if (val) {
                            switch (field_info[i].type)
                            {
                            case FIELD_TYPE_INT8:
                            {
                                ss << (int)*(char *)val;
                            }
                            break;
                            case FIELD_TYPE_INT16:
                            {
                                ss << *(int16_t *)val;
                            }
                            break;
                            case FIELD_TYPE_INT32:
                            {
                                ss << *(int32_t *)val;
                            }
                            break;
                            case FIELD_TYPE_INT64:
                            {
                                ss << *(int64_t *)val;
                            }
                            break;
                            case FIELD_TYPE_DOUBLE:
                            {
                                ss << *(double *)val;
                            }
                            break;
                            case FIELD_TYPE_STRING:
                            {
                                ss << R"(")" << (const char *)val << R"(")";
                            }
                            break;
                            case FIELD_TYPE_DATASET:
                            {
                                ss << to_json((IDataSet *)val);
                            }
                            break;
                            default:
                            {
                                if (conv_)
                                {
                                    ss << conv_(field_info[i].type, val);
                                }
                                else
                                {
                                    ss << R"(")" << val << R"(")";
                                }
                            }
                            break;
                            }
						}
                    }
                    ss << R"(])";
                }
            }
        }
        ss << R"(})";
        return ss.str();
    }

    std::string to_json_ex(IDataSet *dataset)
    {
        std::stringstream ss;
        ss << R"({)";
        {
			bool first_comma = true;
            size_t i = 0, j = 0;
			size_t field_num = dataset->GetFieldCount();
			FieldInfo* field_info = (FieldInfo*)alloca(sizeof(FieldInfo)*field_num);
            field_num = dataset->GetFieldInfo(field_info, field_num);
            
            first_comma = true;
            i = 0, j = 0;
            ss << R"("fields":[)";
            for (; i < field_num; i++)
            {
				if (!first_comma)
				{
					ss << R"(,)";
				}
				else
				{
					first_comma = false;
				}
                ss << R"({)";
				ss << R"("id":)" << (const char*)field_info[i].id << R"(,"type":)" << field_info[i].type;
                ss << R"(})";
            }
            ss << R"(])";

            ss << R"(,)";
            first_comma = true;
            i = 0, j = 0;
            for (; i < field_num; i++)
            {
                size_t field_size = dataset->GetFieldSize(field_info[i].id);
                if (field_size == 0)
                {
                    continue;
                }

				if (!first_comma)
				{
					ss << R"(,)";
				}
				else
				{
					first_comma = false;
				}
				ss << R"(")" << (const char*)field_info[i].id << R"(":)";
                if (field_size == 1)
                {
					void *val = dataset->GetFieldValue(field_info[i].id);
					if (val) {
                        switch (field_info[i].type)
                        {
                        case FIELD_TYPE_INT8:
                        {
                            ss << (int)*(char *)val;
                        }
                        break;
                        case FIELD_TYPE_INT16:
                        {
                            ss << *(int16_t *)val;
                        }
                        break;
                        case FIELD_TYPE_INT32:
                        {
                            ss << *(int32_t *)val;
                        }
                        break;
                        case FIELD_TYPE_INT64:
                        {
                            ss << *(int64_t *)val;
                        }
                        break;
                        case FIELD_TYPE_DOUBLE:
                        {
                            ss << *(double *)val;
                        }
                        break;
                        case FIELD_TYPE_STRING:
                        {
                            ss << R"(")" << (const char *)val << R"(")";
                        }
                        break;
                        case FIELD_TYPE_DATASET:
                        {
                            ss << to_json((IDataSet *)val);
                        }
                        break;
                        default:
                        {
                            if (conv_)
                            {
                                ss << conv_(field_info[i].type, val);
                            }
                            else
                            {
                                ss << R"(")" << val << R"(")";
                            }
                        }
                        break;
                        }
					}
                }
                else
                {
                    ss << R"([)";
                    for (j = 0; j < field_size; j++)
                    {
                        if (j != 0)
                        {
                            ss << R"(,)";
                        }
                        void *val = dataset->GetFieldValue(field_info[i].id, j);
						if (val) {
                            switch (field_info[i].type)
                            {
                            case FIELD_TYPE_INT8:
                            {
                                ss << (int)*(char *)val;
                            }
                            break;
                            case FIELD_TYPE_INT16:
                            {
                                ss << *(int16_t *)val;
                            }
                            break;
                            case FIELD_TYPE_INT32:
                            {
                                ss << *(int32_t *)val;
                            }
                            break;
                            case FIELD_TYPE_INT64:
                            {
                                ss << *(int64_t *)val;
                            }
                            break;
                            case FIELD_TYPE_DOUBLE:
                            {
                                ss << *(double *)val;
                            }
                            break;
                            case FIELD_TYPE_STRING:
                            {
                                ss << R"(")" << (const char *)val << R"(")";
                            }
                            break;
                            case FIELD_TYPE_DATASET:
                            {
                                ss << to_json((IDataSet *)val);
                            }
                            break;
                            default:
                            {
                                if (conv_)
                                {
                                    ss << conv_(field_info[i].type, val);
                                }
                                else
                                {
                                    ss << R"(")" << val << R"(")";
                                }
                            }
                            break;
                            }
						}
                    }
                    ss << R"(])";
                }
            }
        }
        ss << R"(})";
        return ss.str();
    }
};

}