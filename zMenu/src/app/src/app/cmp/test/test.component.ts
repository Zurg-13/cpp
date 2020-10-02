import { Component, OnInit } from '@angular/core';
import { ConfService, AppInfo } from 'src/app/srv/conf.service';

@Component({
  selector: 'app-test',
  templateUrl: './test.component.html',
  styleUrls: ['./test.component.scss']
})
export class TestComponent implements OnInit {

  public info: AppInfo;
  
  constructor(
    private conf_svc: ConfService
  ) { 
    console.log('controller test');
  }

  // Метод для получения информации о приложении через http (используя наш сервис обёртку)
  private getAppInfo(): void {
    this.conf_svc.getAppInfo()
    .then(() => {
      this.info = this.conf_svc.info;
    })
    .catch(() => {
      console.error('Запрос конфигурации не прошел');
    });
  }

  ngOnInit(): void {

    this.conf_svc.load()
    .then(() => {
      console.log('Конфигурация обработана');
      this.getAppInfo();
    })
    .catch(() => {
      console.log('Ошибка получения конфигурации');
    });
  }
}
