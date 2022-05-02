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
		IJObject(std::basic_string<CharT> &&src)
		{
			JSrc = src;
			ClearString();
		}
		virtual std::map<std::basic_string<CharT>, std::unique_ptr<IJObject<CharT>>> Value() = 0;
		std::basic_string<CharT> GetSrc()
		{
			return JSrc;
		}

	protected:
		std::basic_string<CharT> JSrc;
		size_t FindPair(CharT open, CharT close, size_t whom)
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
			return std::basic_string<CharT>::npos;
		}

		void ClearString()
		{
			//удаляем все ненужные символы
			JSrc.erase(std::remove_if(JSrc.begin(), JSrc.end(), [=, isQuotes = false](wchar_t x) mutable
									  {
							if (x == this->charset.quotes)
							{
								isQuotes = !isQuotes;
							}
							return (isQuotes) ? false : 
							x == this->charset.space || x == this->charset.newline || x == this->charset.carret_return; }),
					   JSrc.end());
			// remove \\ separators
			do
			{
				size_t pos = JSrc.find(this->charset.win_path_separator);
				if (pos != std::basic_string<CharT>::npos)
				{
					JSrc.erase(JSrc.begin() + pos);
				}
			} while (JSrc.find(this->charset.win_path_separator) != std::basic_string<CharT>::npos);
		}

		struct CharSet
		{
			constexpr CharSet()
			{
				if constexpr (std::is_same<CharT, char>::value)
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
				if constexpr (std::is_same<CharT, wchar_t>::value)
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
			CharT array_begin;
			CharT array_close;
			CharT quotes;
			CharT newline;
			CharT carret_return;
			CharT space;
			CharT comma;
			CharT closure_begin;
			CharT closure_end;
			CharT colon;
			std::basic_string<CharT> win_path_separator;
		} charset;

		template <typename ToConv>
		std::basic_string<CharT> to_string(ToConv &&conv)
		{
			if constexpr (std::is_same<CharT, char>::value)
			{
				return std::to_string(conv);
			}
			if constexpr (std::is_same<CharT, wchar_t>::value)
			{
				return std::to_wstring(conv);
			}
			return std::basic_string<CharT>();
		}
	};

	template <typename CharT>
	class JValue : public IJObject<CharT>
	{
	public:
		JValue(std::basic_string<CharT> &&src) : IJObject<CharT>(std::move(src)) {}
		virtual std::map<std::basic_string<CharT>, std::unique_ptr<IJObject<CharT>>> Value()
		{
			return std::map<std::basic_string<CharT>, std::unique_ptr<JSON::IJObject<CharT>>>();
		}
	};

	template <typename CharT>
	class JArray : public IJObject<CharT>
	{
	public:
		JArray(std::basic_string<CharT> &&src) : IJObject<CharT>(std::move(src)) {}
		virtual std::map<std::basic_string<CharT>, std::unique_ptr<IJObject<CharT>>> Value()
		{
			size_t end = FindPair(this->charset.array_begin, this->charset.array_close, 0);
			if (end != std::basic_string<CharT>::npos)
			{
				std::map<std::basic_string<CharT>, std::unique_ptr<JSON::IJObject<CharT>>> out;
				
				size_t fblock_start = 1;
				size_t fblock_stop = 0;
				size_t foffset = 1; //указывает на следующий парный символ

				int kcount = 0;

				do
				{
					if (JSrc[fblock_start] == this->charset.closure_begin)
					{
						fblock_stop = FindPair(this->charset.closure_begin, this->charset.closure_end, fblock_start);
						if (fblock_stop != -1)
						{
							// auto src = JSrc.substr(fblock_start, fblock_stop - fblock_start + 1);
							out[to_string(kcount)] = std::make_unique<JObject<CharT>>(
								JSrc.substr(fblock_start, fblock_stop - fblock_start + 1));
							fblock_start = fblock_stop + 2;
						}
					}
					if (JSrc[fblock_start] == this->charset.array_begin)
					{
						fblock_stop = FindPair(this->charset.array_begin, this->charset.array_close, fblock_start);
						if (fblock_stop != -1)
						{
							// auto src = JSrc.substr(fblock_start, fblock_stop - fblock_start + 1);
							out[to_string(kcount)] = std::make_unique<JArray<CharT>>(
								JSrc.substr(fblock_start, fblock_stop - fblock_start + 1));
							fblock_start = fblock_stop + 2;
						}
					}
					if (JSrc[fblock_start] != this->charset.closure_begin && JSrc[fblock_start] != this->charset.array_begin)
					{
						if (JSrc[fblock_start] == this->charset.quotes)
						{
							fblock_start++; //+1 символ т.к. кавычки нам не нужны

							fblock_stop = JSrc.find(this->charset.quotes, fblock_start + 1);
						}
						else
						{
							fblock_stop = JSrc.find(this->charset.comma, fblock_start);
							if (fblock_stop == std::basic_string<CharT>::npos) //если точек нет то читаем до конца
							{
								fblock_stop = end;
								if (fblock_stop <= fblock_start)
									fblock_stop = std::basic_string<CharT>::npos;
							}
						}
						if (fblock_stop != std::basic_string<CharT>::npos)
						{
							if (fblock_stop != fblock_start) //Если они равны, то скорее всего неверный символ
							{
								// auto src = JSrc.substr(fblock_start, fblock_stop - fblock_start);
								out[to_string(kcount)] = std::make_unique<JValue<CharT>>(
									JSrc.substr(fblock_start, fblock_stop - fblock_start));
							}
							fblock_start = fblock_stop + 1;
						}
					}

					kcount++;

				} while (fblock_stop != std::basic_string<CharT>::npos && fblock_stop != -1 && fblock_start < JSrc.length());

				return out;
			}
			return std::map<std::basic_string<CharT>, std::unique_ptr<IJObject<CharT>>>();
		};
	};

	template <typename CharT>
	class JObject : public IJObject<CharT>
	{
	public:
		JObject(std::basic_string<CharT> &&src) : IJObject<CharT>(std::move(src)) {}
		virtual std::map<std::basic_string<CharT>, std::unique_ptr<IJObject<CharT>>> Value()
		{
			size_t end = FindPair(this->charset.closure_begin, this->charset.closure_end, 0);
			if (end != -1)
			{
				std::map<std::basic_string<CharT>, std::unique_ptr<JSON::IJObject<CharT>>> out;

				size_t fcolon = 0;
				size_t foffset = 1; //указывает на следующий парный символ
				do
				{
					fcolon = JSrc.find(this->charset.colon, foffset);
					if (fcolon != std::basic_string<CharT>::npos)
					{
						size_t close = 0;
						int type = 0;

						int shiftNeed = 0;

						if (JSrc[fcolon + 1] == this->charset.closure_begin)
						{
							close = FindPair(this->charset.closure_begin, this->charset.closure_end, fcolon + 1);
							if (close == -1)
								throw std::exception("missing }");
						}
						if (JSrc[fcolon + 1] == this->charset.array_begin)
						{
							type = 1;
							close = FindPair(this->charset.array_begin, this->charset.array_close, fcolon + 1);
							if (close == -1)
								throw std::exception("missing ]");
						}
						if (JSrc[fcolon + 1] != this->charset.closure_begin && JSrc[fcolon + 1] != this->charset.array_begin)
						{
							type = 2;
							if (JSrc[fcolon + 1] == this->charset.quotes)
							{
								shiftNeed = 1;
								close = JSrc.find(this->charset.quotes, fcolon + 2); //т.к. на +1 - начинающие кавычки
								if (close == std::basic_string<CharT>::npos)
									throw std::exception("missing \"");
							}
							else
							{
								close = JSrc.find(this->charset.comma, fcolon) - 1; //-1 - т.к. указывать будет ровно на запятую
								if (close + 1 == std::basic_string<CharT>::npos)
								{
									close = end - 1;
									if (fcolon > close)
										throw std::exception("missing data after :");
								}
							}
						}

						std::basic_string<CharT> src;
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

						auto clean = [=](wchar_t x)
						{
							return x == this->charset.quotes;
						};
						key.erase(std::remove_if(key.begin(), key.end(), clean), key.end());
						switch (type)
						{
						case 0:
						{
							// auto obj = std::make_unique<JObject>(std::move(src));
							out[key] = std::make_unique<JObject<CharT>>(std::move(src));
						}
						break;
						case 1:
						{
							// auto obj = std::make_unique<JArray>(std::move(src));
							out[key] = std::make_unique<JArray<CharT>>(std::move(src));
						}
						break;
						case 2:
						{
							// auto obj = std::make_unique<JValue>(std::move(src));
							out[key] = std::make_unique<JValue<CharT>>(std::move(src));
							// out.emplace(key, std::move(src));
						}
						break;
						}
					}
				} while (fcolon != std::basic_string<CharT>::npos);
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
		CharT object;
		CharT array;

	public:
		JSONParser(std::filesystem::path _Path)
		{
			Path = _Path;
			if constexpr (std::is_same<CharT, char>::value)
			{
				object = '{';
				array = '[';
			}
			if constexpr (std::is_same<CharT, wchar_t>::value)
			{
				object = L'{';
				array = L'[';
			}
		}
		// std::unique_ptr<IJObject<CharT>> Parse();
		std::unique_ptr<IJObject<CharT>> Parse()
		{
			UniversalSCParser::UTF8FileParser Parser(Path);
			auto content = Parser.Read();

			for (size_t i = 0; i < content.size(); i++)
			{
				switch (content[i])
				{
				case object:
					return std::make_unique<JSON::JObject<CharT>>(std::move(content));
					break;
				case array:
					return std::make_unique<JSON::JArray<CharT>>(std::move(content));
					break;
				default:
					return std::make_unique<JSON::JValue<CharT>>(std::move(content));
					break;
				}
			}

			throw std::exception("invalid json object");
		}

	private:
		std::filesystem::path Path;

		std::basic_string<CharT> Read()
		{
			size_t size = std::filesystem::file_size(Path);
			auto buffer = std::make_unique<char[]>(size + 1);

			std::ifstream stream(Path.c_str());
			stream.read(buffer.get(), size);

			return std::basic_string<CharT>(buffer.get(), size);
		}
	};

}
#endif // JSON_H