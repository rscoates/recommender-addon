#include "nan.h"
#include "../include/recommender.h"

using namespace std;
using namespace Nan;
using namespace v8;

class TopCFRecommendationsWorker : public AsyncWorker {
public:
	TopCFRecommendationsWorker(Callback * callback, vector<vector<double>> ratings, int rowIndex, int limit) :
		AsyncWorker(callback),
		ratings(ratings),
		rowIndex(rowIndex),
		limit(limit) {}

	void Execute() {
		Recommender recommender;
		this->result = recommender.getTopCFRecommendations(this->ratings, this->rowIndex, this->limit);
	}

	void HandleOKCallback() {
		Local<Array> result = New<v8::Array>();
		for (unsigned i = 0; i < this->result.size(); i++) {
			Local<Object> obj = Nan::New<Object>();
			Local<String> itemIdProp = Nan::New<String>("itemId").ToLocalChecked();
			Local<String> ratingProp = Nan::New<String>("rating").ToLocalChecked();
			obj->Set(itemIdProp, Nan::New<Number>(this->result[i].first));
			obj->Set(ratingProp, Nan::New<Number>(this->result[i].second));

			Nan::Set(result, i, obj);
		}
		Local<Value> argv[] = { result };
		callback->Call(1, argv);
	}

private:
	vector<vector<double>> ratings;
	int rowIndex;
	int limit;
	vector<pair<int, double>> result;
};