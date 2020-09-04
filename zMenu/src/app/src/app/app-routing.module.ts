import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { TestComponent } from './cmp/test/test.component';
import { CtrlComponent } from './cmp/ctrl/ctrl.component';
import { MenuComponent } from './cmp/menu/menu.component';

const routes: Routes = [
  { path: '', component: TestComponent },
  { path: 'ctrl', component: CtrlComponent },
  { path: 'menu', component: MenuComponent }
];



@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
