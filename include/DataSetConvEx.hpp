#pragma once

#include <DataSetConv.hpp>
#include <XUtil/XXml.hpp>

namespace MTP {

class DataSetConvEx : public DataSetConv
{
public:
    using DataSetConv::DataSetConv;

    bool to_xml(boost::property_tree::ptree& xml, IDataSet* dataset)
    {
        size_t i = 0, j = 0;
        size_t field_num = dataset->GetFieldCount();
        FieldInfo *field_info = (FieldInfo *)alloca(sizeof(FieldInfo) * field_num);
        field_num = dataset->GetFieldInfo(field_info, field_num);

        boost::property_tree::ptree fields;
        i = 0, j = 0;
        for (; i < field_num; i++)
        {
            boost::property_tree::ptree field;
            field.put("id", field_info[i].id);
            field.put("type", field_info[i].type);
            fields.push_back(std::make_pair("", field));
        }
        xml.push_back(std::make_pair("fields", fields));

        i = 0, j = 0;
        for (; i < field_num; i++)
        {
            size_t field_size = dataset->GetFieldSize(field_info[i].id);
            if (field_size == 0)
            {
                continue;
            }

            std::string ids = std::to_string(field_info[i].id);
            if (field_size == 1)
            {
                void *val = dataset->GetFieldValue(field_info[i].id);
                if(val) {
                    switch (field_info[i].type)
                    {
                    case FIELD_TYPE_INT8:
                    {
                        xml.put(ids, (int)*(char *)val);
                    }
                    break;
                    case FIELD_TYPE_INT16:
                    {
                        xml.put(ids, *(int16_t *)val);
                    }
                    break;
                    case FIELD_TYPE_INT32:
                    {
                        xml.put(ids, *(int32_t *)val);
                    }
                    break;
                    case FIELD_TYPE_INT64:
                    {
                        xml.put(ids, *(int64_t *)val);
                    }
                    break;
                    case FIELD_TYPE_DOUBLE:
                    {
                        xml.put(ids, *(double *)val);
                    }
                    break;
                    case FIELD_TYPE_STRING:
                    {
                        xml.put(ids, (const char *)val);
                    }
                    break;
                    case FIELD_TYPE_DATASET:
                    {
                        boost::property_tree::ptree tmp;
                        to_xml(tmp, (IDataSet *)val);
                        xml.push_back(std::make_pair(ids, tmp));
                    }
                    break;
                    default:
                    {
                        if (conv_)
                        {
                            xml.put(ids, conv_(field_info[i].type, val));
                        }
                        else
                        {
                            //xml.put(ids, (const char *)val);
                        }
                    }
                    break;
                    }
                }
            }
            else
            {
                boost::property_tree::ptree array;
                for (j = 0; j < field_size; j++)
                {
                    void *val = dataset->GetFieldValue(field_info[i].id, j);
                    boost::property_tree::ptree tmp;
                    if(val) {
                        switch (field_info[i].type)
                        {
                        case FIELD_TYPE_INT8:
                        {
                            tmp.put("", (int)*(char *)val);
                        }
                        break;
                        case FIELD_TYPE_INT16:
                        {
                            tmp.put("", *(int16_t *)val);
                        }
                        break;
                        case FIELD_TYPE_INT32:
                        {
                            tmp.put("", *(int32_t *)val);
                        }
                        break;
                        case FIELD_TYPE_INT64:
                        {
                            tmp.put("", *(int64_t *)val);
                        }
                        break;
                        case FIELD_TYPE_DOUBLE:
                        {
                            tmp.put("", *(double *)val);
                        }
                        break;
                        case FIELD_TYPE_STRING:
                        {
                            tmp.put("", (const char *)val);
                        }
                        break;
                        case FIELD_TYPE_DATASET:
                        {
                            to_xml(tmp, (IDataSet *)val);
                        }
                        break;
                        default:
                        {
                            if (conv_)
                            {
                                tmp.put("", conv_(field_info[i].type, val));
                            }
                            else
                            {
                                //tmp.put("", (const char *)val);
                            }
                        }
                        break;
                        }
                    }
                    array.push_back(std::make_pair("", tmp));
                }
                xml.push_back(std::make_pair(ids, array));
            }
        }
        return true;
    }

    template<class DataSet = CDataSet>
    bool from_xml(const boost::property_tree::ptree& xml, DataSet* dataset)
    {
        auto opt_fields = xml.get_child_optional("fields");
        if(!opt_fields) {
            return false;
        }
        const boost::property_tree::ptree& xml_fields = opt_fields.get();
        size_t i = 0, j = 0;
        size_t field_num = xml_fields.size();
		FieldInfo* field_info = (FieldInfo*)alloca(sizeof(FieldInfo)*field_num);
        BOOST_FOREACH (const boost::property_tree::ptree::value_type &field, xml_fields)
        {
            field_info[i].id = field.second.get<size_t>("id");
            field_info[i].type = field.second.get<size_t>("type");
			i++;
        }
        ASSERT(i==field_num);
        dataset->Init(field_info, field_num);
		for(i = 0; i < field_num; i++)
		{
            std::string ids = std::to_string(field_info[i].id);
            auto opt_field = xml.get_child_optional(ids);
            if(!opt_field) {
                continue;
            }
            const boost::property_tree::ptree& xml_field = opt_field.get();
            size_t field_size = xml_field.size();
            switch (field_info[i].type)
            {
            //case FIELD_TYPE_CHAR:
            //	break;
            //case FIELD_TYPE_BYTE:
            //	break;
            case FIELD_TYPE_INT8:
                {
                    if(field_size <= 1) {
                        dataset->SetFieldValue(field_info[i].id,xml_field.get_value<uint8_t>());
                    } else {
                        std::vector<uint8_t> array(field_size);
						j = 0;
                        BOOST_FOREACH (const boost::property_tree::ptree::value_type &field, xml_field)
                        {
                            array[j] = field.second.get_value<uint8_t>();
							j++;
                        }
                        dataset->SetFieldValue(field_info[i].id, &array[0], array.size());
                    }
                }
                break;
            case FIELD_TYPE_INT16:
                {
                    if(field_size <= 1) {
                        dataset->SetFieldValue(field_info[i].id,xml_field.get_value<uint16_t>());
                    } else {
                        std::vector<uint16_t> array(field_size);
						j = 0;
                        BOOST_FOREACH (const boost::property_tree::ptree::value_type &field, xml_field)
                        {
                            array[j] = field.second.get_value<uint16_t>();
							j++;
                        }
                        dataset->SetFieldValue(field_info[i].id, &array[0], array.size());
                    }
                }
                break;
            case FIELD_TYPE_INT32:
                {
                    if(field_size <= 1) {
                        dataset->SetFieldValue(field_info[i].id,xml_field.get_value<uint32_t>());
                    } else {
                        std::vector<uint32_t> array(field_size);
						j = 0;
                        BOOST_FOREACH (const boost::property_tree::ptree::value_type &field, xml_field)
                        {
                            array[j] = field.second.get_value<uint32_t>();
							j++;
                        }
                        dataset->SetFieldValue(field_info[i].id, &array[0], array.size());
                    }
                }
                break;
            case FIELD_TYPE_INT64:
                {
                    if(field_size <= 1) {
                        dataset->SetFieldValue(field_info[i].id,xml_field.get_value<uint64_t>());
                    } else {
                        std::vector<uint64_t> array(field_size);
						j = 0;
                        BOOST_FOREACH (const boost::property_tree::ptree::value_type &field, xml_field)
                        {
                            array[j] = field.second.get_value<uint64_t>();
							j++;
                        }
                        dataset->SetFieldValue(field_info[i].id, &array[0], array.size());
                    }
                }
                break;
            case FIELD_TYPE_DOUBLE:
                {
                    if(field_size <= 1) {
                        dataset->SetFieldValue(field_info[i].id,xml_field.get_value<double>());
                    } else {
                        std::vector<double> array(field_size);
						j = 0;
                        BOOST_FOREACH (const boost::property_tree::ptree::value_type &field, xml_field)
                        {
                            array[j] = field.second.get_value<double>();
							j++;
                        }
                        dataset->SetFieldValue(field_info[i].id, &array[0], array.size());
                    }
                }
                break;
            case FIELD_TYPE_STRING:
                {
                    if(field_size <= 1) {
                        dataset->SetFieldValue(field_info[i].id, xml_field.get_value<std::string>());
                    } else {
                        std::vector<std::string> array(field_size);
						j = 0;
                        BOOST_FOREACH (const boost::property_tree::ptree::value_type &field, xml_field)
                        {
                            array[j] = field.second.get_value<std::string>();
							j++;
                        }
                        dataset->SetFieldValue(field_info[i].id, &array[0], array.size());
                    }
                }
                break;
            case FIELD_TYPE_DATASET:
                {
                    if(field_size <= 1) {
                        CDataSet* tmp = new CDataSet();
                        from_xml(xml_field, tmp);
                        dataset->SetFieldValue(field_info[i].id, tmp);
                    } else {
                        CDataSet** array = new CDataSet*[field_size];
						j = 0;
                        BOOST_FOREACH (const boost::property_tree::ptree::value_type &field, xml_field)
                        {
                            array[j] = new CDataSet();
                            from_xml(field.second, array[j]);
							j++;
                        }
                        dataset->SetFieldValue(field_info[i].id, array, field_size);
                    }
                }
                break;
            default:
                {
                    /*if(field_size <= 1) {
                        dataset->SetFieldValue(field_info[i].id, xml_field.get_value<std::string>());
                    } else {
                        std::vector<std::string> array(field_size);
						j = 0;
                        BOOST_FOREACH (const boost::property_tree::ptree::value_type &field, xml_field)
                        {
                            array[j] = field.second.get_value<std::string>();
							j++;
                        }
                        dataset->SetFieldValue(field_info[i].id, &array[0], array.size());
                    }*/
                }
                break;
            }
		}
        return true;
    }

    template<class StrDataSet = CStrDataSet>
    bool from_xml_ex(const boost::property_tree::ptree& xml, StrDataSet* dataset)
    {
        auto opt_fields = xml.get_child_optional("fields");
        if(!opt_fields) {
            return false;
        }
        const boost::property_tree::ptree& xml_fields = opt_fields.get();
        size_t i = 0, j = 0;
        size_t field_num = xml_fields.size();
		FieldInfo* field_info = (FieldInfo*)alloca(sizeof(FieldInfo)*field_num);
        BOOST_FOREACH (const boost::property_tree::ptree::value_type &field, xml_fields)
        {
            FieldInfo field_info = {0};
            field_info.id = (size_t)field.second.get<std::string>("id").c_str();
            field_info.type = field.second.get<size_t>("type");
			dataset->SetFieldInfo(&field_info);
        }
        field_num = dataset->GetFieldInfo(field_info, field_num);
		for(i = 0; i < field_num; i++)
		{
            const char* ids = (const char*)field_info[i].id;
            auto opt_field = xml.get_child_optional(ids);
            if(!opt_field) {
                continue;
            }
            const boost::property_tree::ptree& xml_field = opt_field.get();
            size_t field_size = xml_field.size();
            switch (field_info[i].type)
            {
            //case FIELD_TYPE_CHAR:
            //	break;
            //case FIELD_TYPE_BYTE:
            //	break;
            case FIELD_TYPE_INT8:
                {
                    if(field_size <= 1) {
                        dataset->SetFieldValue(field_info[i].id,xml_field.get_value<uint8_t>());
                    } else {
                        std::vector<uint8_t> array(field_size);
						j = 0;
                        BOOST_FOREACH (const boost::property_tree::ptree::value_type &field, xml_field)
                        {
                            array[j] = field.second.get_value<uint8_t>();
							j++;
                        }
                        dataset->SetFieldValue(field_info[i].id, &array[0], array.size());
                    }
                }
                break;
            case FIELD_TYPE_INT16:
                {
                    if(field_size <= 1) {
                        dataset->SetFieldValue(field_info[i].id,xml_field.get_value<uint16_t>());
                    } else {
                        std::vector<uint16_t> array(field_size);
						j = 0;
                        BOOST_FOREACH (const boost::property_tree::ptree::value_type &field, xml_field)
                        {
                            array[j] = field.second.get_value<uint16_t>();
							j++;
                        }
                        dataset->SetFieldValue(field_info[i].id, &array[0], array.size());
                    }
                }
                break;
            case FIELD_TYPE_INT32:
                {
                    if(field_size <= 1) {
                        dataset->SetFieldValue(field_info[i].id,xml_field.get_value<uint32_t>());
                    } else {
                        std::vector<uint32_t> array(field_size);
						j = 0;
                        BOOST_FOREACH (const boost::property_tree::ptree::value_type &field, xml_field)
                        {
                            array[j] = field.second.get_value<uint32_t>();
							j++;
                        }
                        dataset->SetFieldValue(field_info[i].id, &array[0], array.size());
                    }
                }
                break;
            case FIELD_TYPE_INT64:
                {
                    if(field_size <= 1) {
                        dataset->SetFieldValue(field_info[i].id,xml_field.get_value<uint64_t>());
                    } else {
                        std::vector<uint64_t> array(field_size);
						j = 0;
                        BOOST_FOREACH (const boost::property_tree::ptree::value_type &field, xml_field)
                        {
                            array[j] = field.second.get_value<uint64_t>();
							j++;
                        }
                        dataset->SetFieldValue(field_info[i].id, &array[0], array.size());
                    }
                }
                break;
            case FIELD_TYPE_DOUBLE:
                {
                    if(field_size <= 1) {
                        dataset->SetFieldValue(field_info[i].id,xml_field.get_value<double>());
                    } else {
                        std::vector<double> array(field_size);
						j = 0;
                        BOOST_FOREACH (const boost::property_tree::ptree::value_type &field, xml_field)
                        {
                            array[j] = field.second.get_value<double>();
							j++;
                        }
                        dataset->SetFieldValue(field_info[i].id, &array[0], array.size());
                    }
                }
                break;
            case FIELD_TYPE_STRING:
                {
                    if(field_size <= 1) {
                        dataset->SetFieldValue(field_info[i].id, xml_field.get_value<std::string>());
                    } else {
                        std::vector<std::string> array(field_size);
						j = 0;
                        BOOST_FOREACH (const boost::property_tree::ptree::value_type &field, xml_field)
                        {
                            array[j] = field.second.get_value<std::string>();
							j++;
                        }
                        dataset->SetFieldValue(field_info[i].id, &array[0], array.size());
                    }
                }
                break;
            case FIELD_TYPE_DATASET:
                {
                    if(field_size <= 1) {
                        CDataSet* tmp = new CDataSet();
                        from_xml(xml_field, tmp);
                        dataset->SetFieldValue(field_info[i].id, tmp);
                    } else {
                        CDataSet** array = new CDataSet*[field_size];
						j = 0;
                        BOOST_FOREACH (const boost::property_tree::ptree::value_type &field, xml_field)
                        {
                            array[j] = new CDataSet();
                            from_xml(field.second, array[j]);
							j++;
                        }
                        dataset->SetFieldValue(field_info[i].id, array, field_size);
                    }
                }
                break;
            default:
                {
                    /*if(field_size <= 1) {
                        dataset->SetFieldValue(field_info[i].id, xml_field.get_value<std::string>());
                    } else {
                        std::vector<std::string> array(field_size);
						j = 0;
                        BOOST_FOREACH (const boost::property_tree::ptree::value_type &field, xml_field)
                        {
                            array[j] = field.second.get_value<std::string>();
							j++;
                        }
                        dataset->SetFieldValue(field_info[i].id, &array[0], array.size());
                    }*/
                }
                break;
            }
		}
        return true;
    }
};

}