namespace portaible
{
	namespace Loader
	{
		template<typename T>
		class RegisterHelper
		{
		public:
			RegisterHelper(std::string name)
			{
				PORTAIBLE_RUNTIME->loader.registerLoader<T>(name);
			}
		};
	}
}
