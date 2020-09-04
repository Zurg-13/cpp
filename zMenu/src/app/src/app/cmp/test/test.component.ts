import { Component, OnInit } from '@angular/core';
import { ConfigService, AppInfo } from 'src/app/srv/config.service';

@Component({
  selector: 'app-test',
  templateUrl: './test.component.html',
  styleUrls: ['./test.component.scss']
})
export class TestComponent implements OnInit {

  public appInfo: AppInfo;

  constructor(
    private configService: ConfigService
    ) { 
    console.log('controller test');
  }

  // Метод для получения информации о приложении через http (используя наш сервис обёртку)
  private getAppInfo(): void {
    this.configService.getAppInfo()
    .then(() => {
      this.appInfo = this.configService.appInfo;
    })
    .catch(() => {

    });
  }

  ngOnInit(): void {
    // Послать запрос на сервер и получить версию приложения
    this.configService.load()
    .then(() => {
      console.log('Конфигурация обработана');
      this.getAppInfo();
    })
    .catch(() => {
      console.log('Ошибка получения конфигурации');
    });
  }

}
