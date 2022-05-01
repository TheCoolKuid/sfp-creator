#ifndef JSON_H
#define JSON_H

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <regex>
#include <filesystem>
#include <fstream>

namespace JSON
{
	template <typename CharT>
	class IJObject
	{
	public:
		IJObject(std::basic_string<T> &&src)
		{
			JSrc = src;
		}
		virtual std::map<std::basic_string<T>, std::unique_ptr<IJObject>> Value() = 0;
		std::basic_string<T> GetSrc()
		{
			return JSrc;
		}

	protected:
		std::basic_string<T> JSrc;
		size_t FindPair(T open, T close, size_t whom)
		{
			int oc = 0;
			for (size_t i = whom; i < JSrc.size(); i++)
			{
				if (JSrc[i] == open)
				{
					oc++;
				}
				if (JSrc[i] == close)
				{
					oc--;
					if (!oc)
					{
						return i;
					}
				}
			}
			return std::basic_string<T>::npos;
		}

		struct CharSet
		{
			constexpr CharSet()
			{
				if constexpr (std::is_same<T, char>::value)
				{
					array_begin = '[';
					array_close = ']';
					quotes = '\"';
					newline = '\n';
					carret_return = '\r';
					space = ' ';
					comma = ',';
					closure_begin = '{';
					closure_end = '}';
					win_path_separator = "\\\\";
					colon = ':';
				}
				if constexpr (std::is_same<T, wchar_t>::value)
				{
					array_begin = L'[';
					array_close = L']';
					quotes = L'\"';
					newline = L'\n';
					carret_return = L'\r';
					space = L' ';
					comma = L',';
					closure_begin = L'{';
					closure_end = L'}';
					win_path_separator = L"\\\\";
					colon = L':';
				}
			}
			T array_begin;
			T array_close;
			T quotes;
			T newline;
			T carret_return;
			T space;
			T comma;
			T closure_begin;
			T closure_end;
			T colon;
			std::basic_string<T> win_path_separator;
		}

		constexpr CharSet charset;

		inline template <typename ToConv>
		std::basic_string<T> to_string(ToConv &&conv)
		{
			if constexpr (std::is_same<T, char>::value)
			{
				return std::to_string(conv);
			}
			if constexpr (std::is_same<T, wchar_t>::value)
			{
				return std::to_wstring(conv);
			}
			return std::basic_string<T>();
		}
	};

	template <typename CharT>
	class JValue : public IJObject
	{
	public:
		JValue(std::basic_string<T> &&src) : IJObject(std::move(src)) {}
		virtual std::map<std::basic_string<T>, std::unique_ptr<IJObject>> Value()
		{
			return std::map<std::basic_string<T>, std::unique_ptr<JSON::IJObject>>();
		}
	};

	template <typename CharT>
	class JArray : public IJObject
	{
	public:
		JArray(std::basic_string<T> &&src) : IJObject(std::move(src)) {}
		virtual std::map<std::basic_string<T>, std::unique_ptr<IJObject>> Value()
		{
			size_t end = FindPair(charset.array_begin, charset.array_close, 0);
			if (end != std::basic_string<T>::npos)
			{
				std::map<std::wstring, std::unique_ptr<JSON::IJObject>> out;
				//удаляем все ненужные символы
				JSrc.erase(std::remove_if(JSrc.begin(), JSrc.end(), [isQuotes = false](wchar_t x) mutable
										  {
					if (x == charset.quotes)
					{
						isQuotes = !isQuotes;
					}
					return (isQuotes) ? false : x == charset.space || x == charset.newline || x == charset.carret_return; }),
						   JSrc.end());

				size_t fblock_start = 1;
				size_t fblock_stop = 0;
				size_t foffset = 1; //указывает на следующий парный символ

				int kcount = 0;

				do
				{
					switch (JSrc[fblock_start])
					{
					case charset.closure_begin:
					{
						fblock_stop = FindPair(charset.closure_begin, charset.closure_end, fblock_start);
						if (fblock_stop != -1)
						{
							// auto src = JSrc.substr(fblock_start, fblock_stop - fblock_start + 1);
							out[std::to_string(kcount)] = std::make_unique<JObject>(
								JSrc.substr(fblock_start, fblock_stop - fblock_start + 1));
							fblock_start = fblock_stop + 2;
						}
					}
					break;
					case charset.array_begin:
					{
						fblock_stop = FindPair(charset.array_begin, charset.array_close, fblock_start);
						if (fblock_stop != -1)
						{
							// auto src = JSrc.substr(fblock_start, fblock_stop - fblock_start + 1);
							out[std::to_string(kcount)] = std::make_unique<JArray>(
								JSrc.substr(fblock_start, fblock_stop - fblock_start + 1));
							fblock_start = fblock_stop + 2;
						}
					}
					break;
					default:
					{
						if (JSrc[fblock_start] == charset.quotes)
						{
							fblock_start++; //+1 символ т.к. кавычки нам не нужны

							fblock_stop = JSrc.find(charset.quotes, fblock_start + 1);
						}
						else
						{
							fblock_stop = JSrc.find(charset.comma, fblock_start);
							if (fblock_stop == std::basic_string<T>::npos) //если точек нет то читаем до конца
							{
								fblock_stop = end;
								if (fblock_stop <= fblock_start)
									fblock_stop = std::basic_string<T>::npos;
							}
						}
						if (fblock_stop != std::basic_streambuf<T>::npos)
						{
							if (fblock_stop != fblock_start) //Если они равны, то скорее всего неверный символ
							{
								// auto src = JSrc.substr(fblock_start, fblock_stop - fblock_start);
								out[std::to_ws(kcount)] = std::make_unique<JValue>(
									JSrc.substr(fblock_start, fblock_stop - fblock_start));
							}
							fblock_start = fblock_stop + 1;
						}
					}
					break;
					}

					kcount++;

				} while (fblock_stop != std::basic_string<T>::npos && fblock_stop != -1 && fblock_start < JSrc.length());

				return out;
			}
			return std::map<std::basic_string<T>, std::unique_ptr<IJObject>>();
		};

		template <typename CharT>
		class JObject : public IJObject
		{
		public:
			JObject(std::basic_string<T> &&src) : IJObject(std::move(src)) {}
			virtual std::map<std::basic_string<T>, std::unique_ptr<IJObject>> Value()
			{

				size_t end = FindPair(charset.closure_begin, charset.closure_end, 0);
				if (end != -1)
				{
					std::map<std::basic_string<T>, std::unique_ptr<JSON::IJObject>> out;
					//удаляем все ненужные символы
					JSrc.erase(std::remove_if(JSrc.begin(), JSrc.end(), [isQuotes = false](wchar_t x) mutable
											  {
							if (x == charset.quotes)
							{
								isQuotes = !isQuotes;
							}
							return (isQuotes) ? false : x == charset.space || x == charset.newline || x == charset.carret_retrurn; }),
							   JSrc.end());

					do
					{
						size_t pos = JSrc.find(charset.win_path_separator);
						if (pos != std::wstring::npos)
						{
							JSrc.erase(JSrc.begin() + pos);
						}
					} while (JSrc.find(charset.win_path_separator) != std::wstring::npos);

					size_t fcolon = 0;
					size_t foffset = 1; //указывает на следующий парный символ
					do
					{
						fcolon = JSrc.find(charset.colon, foffset);
						if (fcolon != std::wstring::npos)
						{
							size_t close = 0;
							int type = 0;

							int shiftNeed = 0;

							switch (JSrc[fcolon + 1])
							{
							case charset.closure_begin:
								close = FindPair(charset.closure_begin, charset.closure_end, fcolon + 1);
								if (close == -1)
									throw std::exception("missing }");
								break;
							case charset.array_begin:
								type = 1;
								close = FindPair(charset.array_begin, charset.array_close, fcolon + 1);
								if (close == -1)
									throw std::exception("missing ]");
								break;
							default:
								type = 2;
								if (JSrc[fcolon + 1] == charset.quotes)
								{
									shiftNeed = 1;
									close = JSrc.find(charset.quotes, fcolon + 2); //т.к. на +1 - начинающие кавычки
									if (close == std::wstring::npos)
										throw std::exception("missing \"");
								}
								else
								{
									close = JSrc.find(charset.comma, fcolon) - 1; //-1 - т.к. указывать будет ровно на запятую
									if (close + 1 == std::wstring::npos)
									{
										close = end - 1;
										if (fcolon > close)
											throw std::exception("missing data after :");
									}
								}
								break;
							}

							std::basic_string<T> src;
							if (type == 2 && shiftNeed)
							{
								src = JSrc.substr(fcolon + 2, close - (fcolon + 2)); //+2 т.к. избегаем \"
							}
							else
							{
								src = JSrc.substr(fcolon + 1, close - fcolon);
							}

							// auto src = JSrc.substr(fcolon + 1, close - fcolon);
							auto key = JSrc.substr(foffset, fcolon - foffset);

							foffset = close + 2; //+2 т.к. } , {

							auto clean = [](wchar_t x)
							{
								return x == charset.quotes;
							};
							key.erase(std::remove_if(key.begin(), key.end(), clean), key.end());
							switch (type)
							{
							case 0:
							{
								// auto obj = std::make_unique<JObject>(std::move(src));
								out[key] = std::make_unique<JObject>(std::move(src));
							}
							break;
							case 1:
							{
								// auto obj = std::make_unique<JArray>(std::move(src));
								out[key] = std::make_unique<JArray>(std::move(src));
							}
							break;
							case 2:
							{
								// auto obj = std::make_unique<JValue>(std::move(src));
								out[key] = std::make_unique<JValue>(std::move(src));
								// out.emplace(key, std::move(src));
							}
							break;
							}
						}
					} while (fcolon != std::basic_string<T>::npos);
					return out;
				}
				else
				{
					throw std::exception("Invalid json object");
				}
			}
		};

		template <typename CharT>
		class JSONParser
		{
			T object;
			T array;
		public:
			JSONParser(std::filesystem::path _Path)
			{
				Path = _Path;
				if constexpr (std::is_same<T, char> :: value) {
					object = '{';
					array = '[';
				}
				if constexpr (std::is_same<T, wchar_t> :: value) {
					object = L'{';
					array = L'[';
				}					
			}
			// std::unique_ptr<IJObject> Parse();
			std::unique_ptr<IJObject> Parse()
			{
				UniversalSCParser::UTF8FileParser Parser(Path);
				auto content = Parser.Read();

				for (size_t i = 0; i < content.size(); i++)
				{
					switch (content[i])
					{
					case object:
						return std::make_unique<JSON::JObject>(std::move(content));
						break;
					case array:
						return std::make_unique<JSON::JArray>(std::move(content));
						break;
					default:
						return std::make_unique<JSON::JValue>(std::move(content));
						break;
					}
				}

				throw std::exception("invalid json object");
			}

		private:
			std::filesystem::path Path;

			std::basic_string<T> Read() {
				size_t size = std::filesystem::file_size(Path);
				auto buffer = std::make_unique<char[]>(size + 1);

				std::ifstream stream(Path.c_str());
				stream.read(buffer.get(), size);

				return std::basic_string<T>(buffer.get(), size);				
			}
		};
	};
}
#endif //JSON_H