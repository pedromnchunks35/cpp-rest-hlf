#ifndef HANDLING_IMPL
#define HANDLING_IMPL
#include <handling.hpp>
#include <iostream>
#include <debug.hpp>
class handlingImpl : public handling
{
public:
    //? Register main route
    void register_main_route(std::string main_route,
                             RegisterRoutesErrors &ec) override
    {
        if (m_routes == nullptr)
        {
            ec = RegisterRoutesErrors::INITIALIZATION_ERROR;
            return;
        }
        //? CASE IT IS SIMPLY THE INIT WE SIMPLY CHANGE THE ROUTE IN IT
        if (m_size_main_routes == 1)
        {
            if ((*m_routes)[0].empty())
            {
                (*m_routes)[0] = main_route;
                return;
            }
        }
        //? CHECK IF THE ROUTE IS ALREADY ON THE MEMORY
        if (contains_route(main_route))
        {
            ec = RegisterRoutesErrors::DUPLICATED;
            return;
        }
        //? JUST ADD MORE MEMORY TO THE ARRAY
        m_size_main_routes++;
        std::string *newArr = new std::string[m_size_main_routes];
        for (uint8_t i = 0; i < m_size_main_routes - 1; i++)
        {
            newArr[i] = std::move((*m_routes)[i]);
        }
        newArr[m_size_main_routes - 1] = main_route;
        delete[] (*m_routes);
        (*m_routes) = newArr;
    }
    //? Register sub route
    void register_sub_route(std::string main_route,
                            RouteInfo &sub_route,
                            RegisterRoutesErrors &ec) override
    {
        //? CASE THE MAIN ROUTE DOES NOT EXIST PUT A ERROR
        if (!contains_route(main_route))
        {
            LOG_W("Error finding a suitable main_route");
            ec = RegisterRoutesErrors::MAIN_ROUTE_DOES_NOT_EXIST;
            return;
        }
        //? CASE THE SUB ROUTE ALREADY EXITS PUT IT AS DUPLICATED
        if (contains_sub_route(main_route, sub_route.path))
        {
            LOG_W("Error finding a suitable sub_route");
            ec = RegisterRoutesErrors::DUPLICATED;
            return;
        }
        //? Check if the key exists
        bool contains_key_sub = contains_key_sub_route(main_route);
        //? WE KNOW THAT THE SUB ROUTE DOES NOT EXIST
        uint8_t &size_sub_route = (*m_sub_routes_size)[main_route];
        if (contains_key_sub)
        {
            size_sub_route++;
        }
        else
        {
            size_sub_route = 1;
        }
        //? CREATE THE NEW ARRAY
        RouteInfo *newArr = new RouteInfo[size_sub_route];
        for (uint8_t i = 0; i < size_sub_route - 1; i++)
        {
            newArr[i] = (*m_sub_routes)[main_route][i];
        }
        newArr[size_sub_route - 1] = std::move(sub_route);
        //? DELETE THE OLDER ONE (IN CASE IT DID NOT EXIST BEFORE DELETING A nullptr should not be a problem)
        if (size_sub_route != 1)
        {
            delete[] (*m_sub_routes)[main_route];
        }
        //? PUT THE NEWLY CREATED ARRAY ON THE POSITION OF THE OLDEST ONE
        (*m_sub_routes)[main_route] = std::move(newArr);
    }
    //? Making the actual handling of calls according to the type of request and
    // the path for the request
    std::function<http::message_generator(http::request<http::string_body>)>
    handle_by_mapping(std::string main_route, std::string sub_route,
                      http::verb method, RegisterRoutesErrors &err) override
    {
        //? CHECK IF THE ROUTES EXIST OR NOT
        if (!contains_route(main_route))
        {
            LOG_W("Error in handling by mapping cannot find the route");
            err = RegisterRoutesErrors::MAIN_ROUTE_DOES_NOT_EXIST;
            return nullptr;
        }
        //? CHECK IF SUBROUTE EXISTS
        if (!contains_sub_route(main_route, sub_route))
        {
            LOG_W("Error in the handling cannot find the sub_route");
            err = RegisterRoutesErrors::SUB_ROUTE_DOES_NOT_EXIST;
            return nullptr;
        }
        //? GET ROUTE INFO (IT CAN RETURN NULL IF THE METHOD DOES NOT MATCH)
        RouteInfo *route_info = get_route_info_by_main_and_sub_path(main_route, sub_route, method);
        if (route_info == nullptr)
        {
            LOG_W("Error in the handling cannot find a match in method");
            err = RegisterRoutesErrors::ROUTE_WITH_METHOD_DONT_MATCH;
            return nullptr;
        }
        return (*route_info).handler;
    }
    bool isInit()
    {
        return initialized;
    }

private:
    RouteInfo *get_route_info_by_main_and_sub_path(std::string main_route, std::string sub_route, http::verb method)
    {
        uint8_t sub_route_map_size = (*m_sub_routes_size)[main_route];
        for (uint8_t i = 0; i < sub_route_map_size; i++)
        {
            if ((*m_sub_routes)[main_route][i].path == sub_route && (*m_sub_routes)[main_route][i].method == method)
            {
                return &(*m_sub_routes)[main_route][i];
            }
        }
        return nullptr;
    }
    bool contains_route(const std::string to_search)
    {
        for (uint8_t i = 0; i < m_size_main_routes; i++)
        {
            if ((*m_routes)[i] == to_search)
            {
                return true;
            }
        }
        return false;
    }
    bool contains_key_sub_route(const std::string main_route)
    {
        //? WE SHOULD CHECK IF THE KEY ACTUALLY EXISTS
        auto it = (*m_sub_routes).find(main_route);
        if (it == (*m_sub_routes).end())
        {
            return false;
        }
        return true;
    }
    bool contains_sub_route(const std::string main_route, const std::string to_search)
    {
        //? WE SHOULD CHECK IF THE KEY ACTUALLY EXISTS
        bool contains_key = contains_key_sub_route(main_route);
        if (!contains_key)
        {
            return contains_key;
        }
        //? The key exists so it means there is items inside of it,lets get the size then
        uint8_t size_sub_route = (*m_sub_routes_size)[main_route];
        for (uint8_t i = 0; i < size_sub_route; i++)
        {
            if ((*m_sub_routes)[main_route][i].path == to_search)
            {
                return true;
            }
        }
        return false;
    }
};
#endif