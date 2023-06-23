#pragma once
#include <sstream>

namespace myspace{
    // Thread safe singleton
    template <typename T>
    class Singleton 
    {
    public:
        static T& self()
        {
            static T instance;
            return instance;
        }

        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;
        
    protected:
        Singleton() {}
        ~Singleton() {}
    };

    class Tools : public Singleton<Tools>
    {
    public:
        std::string GetLoveGraphic()
        {
            std::stringstream ss;
            ss << "\n";

            int  i, j, k, l, m;
            char c = '*';	

            for (i = 1; i <= 3; i++)
            {	
                for (j = 1; j <= 32 - 2 * i; j++)
                    ss << (" ");	
                for (k = 1; k <= 4 * i + 1; k++)
                    ss << ("%c", c);
                for (l = 1; l <= 13 - 4 * i; l++)
                    ss << (" ");
                for (m = 1; m <= 4 * i + 1; m++)
                    ss << ("%c", c);
                ss << ("\n");
            }

            for (i = 1; i <= 3; i++)
            {	
                for (j = 1; j <= 24 + 1; j++)
                    ss << (" ");
                for (k = 1; k <= 29; k++)
                    ss << ("%c", c);
                ss << ("\n");
            }

            for (i = 7; i >= 1; i--)
            {	
                for (j = 1; j <= 40 - 2 * i; j++)
                    ss << (" ");
                for (k = 1; k <= 4 * i - 1; k++)
                    ss << ("%c", c);
                ss << ("\n");
            }

            for (i = 1; i <= 39; i++)
                ss << (" ");

            ss << ("%c\n", c);
            ss << ("\n");

            return ss.str();
        }
    };
   
}