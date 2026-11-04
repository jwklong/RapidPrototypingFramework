#pragma once

namespace Boy
{
	class HttpRequest;
	class HttpResponse;

	class HttpResponseHandler
	{
	public:
		HttpResponseHandler() {}
		virtual ~HttpResponseHandler() {}

		virtual void handleHttpResponse(HttpRequest *request, HttpResponse *response) = 0;
	};
}
