'use strict';

angular.module('app', ['ngResource'])
.factory('requestService', ['$http', '$resource', '$q', function($http, $resource, $q){

      var service = {
          api: resourceApi
      };

      var transformResponse = function(data, headers, statusCode){
          var response = {}
          try
          {
             response.data = angular.fromJson(data);
          }
          catch(e)
          {
             response.data = data;
          }
          response.headers = headers();
          response.status = statusCode;
          return response;
      };

      var headers = {};

      // Общие методы CREATE (POST), QUERY (GET), UPDATE (PUT), DELETE (DELETE)
      service.createMethod = {
          method: 'POST',
          headers: headers,
          transformResponse: transformResponse
      };

      service.queryMethod = {
          method: 'GET',
          headers: headers,
          transformResponse: transformResponse,
          isArray: false
      };

      service.updateMethod = {
          method: 'PUT',
          headers: headers,
          transformResponse: transformResponse
      };

      service.deleteMethod = {
          method: 'DELETE',
          headers: headers,
          transformResponse: transformResponse
      };

      // Общий объект с набором стандартных методов
      var methodObject = {
        create: service.createMethod,
        query: service.queryMethod,
        update: service.updateMethod,
        delete: service.deleteMethod
      };

      // Основной конструктор resource для работы с API
      function resourceApi(url, params, methods) {
        var params = params || {};
        var methods = methods || methodObject;
        return $resource(url, params, methods);
      }

      return service;

}])
.controller('pageController', ['$scope', 'requestService', function($scope, requestService){
    console.log('pageController init');

    var self = this;
    this.list = [];
    this.sum = 0;

    var config = {
        getUrl: 'list'
    }

    var calcSum = function() {
        self.sum = 0;
        angular.forEach(self.list, function(item){
            if(item.act == 1 && item.sel == 1) {
                self.sum += parseFloat(item.prc);
            }
        });
    }

    this.clear = function() {
        angular.forEach(self.list, function(item){
            item.sel = 0;
        });
        calcSum();
    }

    // Обработка списка элементов и добавление дополнительных методов
    var prepareResponse = function(data) {
        angular.forEach(data, function(item){
            item.act = parseInt(item.act);
            item.sel = 0;

            item.toggle = function() {
                this.sel = 1 - this.sel;
                calcSum();
            }
        });
        self.list = data;
        console.log(self.list);
    }

    this.getList = function() {
        var queryMethod = angular.copy(requestService.queryMethod);
        requestService.api(config.getUrl, {}, {query: queryMethod}).query().$promise
        .then(function(response){
            console.log(response);
            prepareResponse(response.data);
        })
        .catch(function(error){
            console.log(error);
        })

/*
        // FIXME: После доработки удалить
        .finally(function(){
            var data = angular.fromJson('[{"nme":"Название", "nte":"подробное описание", "prc":"999.99", "sel":"0", "act":"1"}, {"nme":"Название", "nte":"подробное описание", "prc":"999.99", "sel":"0", "act":"1"}, {"nme":"Название", "nte":"подробное описание", "prc":"999.99", "sel":"0", "act":"1"}, {"nme":"Название", "nte":"подробное описание", "prc":"999.99", "sel":"0", "act":"1"}, {"nme":"Название", "nte":"подробное описание", "prc":"999.99", "sel":"0", "act":"1"}, {"nme":"Название", "nte":"подробное описание", "prc":"999.99", "sel":"0", "act":"1"}, {"nme":"Название", "nte":"подробное описание", "prc":"999.99", "sel":"0", "act":"1"}, {"nme":"Название", "nte":"подробное описание", "prc":"999.99", "sel":"0", "act":"1"}, {"nme":"Название", "nte":"подробное описание", "prc":"999.99", "sel":"0", "act":"1"}, {"nme":"Название", "nte":"подробное описание", "prc":"999.99", "sel":"0", "act":"1"}]');
            prepareResponse(data);
        });
*/
    }

    this.getList();
}]);