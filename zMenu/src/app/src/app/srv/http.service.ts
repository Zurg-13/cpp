import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders, HttpParams } from '@angular/common/http';
import { Observable } from 'rxjs';

/*
  Сервис для взаимодействия с сервером по протоколу http
*/

@Injectable({
  providedIn: 'root'
})
export class HttpService {

  constructor(private http: HttpClient) { }

  // Метод отправки GET запроса по указанному URL
  public get(url: string, params?: HttpParams): Observable<Object> {
    const totalParams: HttpParams = params != null ? params : new HttpParams();
    const headers: HttpHeaders = new HttpHeaders();
    headers.append('Content-Type', 'application/json');
    // Пример добавления параметров руками
    // totalParams.append("paramKey", "paramValue");

    return this.http.get(url, { headers: headers, params: totalParams });
  }
}
