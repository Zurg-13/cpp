import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { TestComponent } from './cmp/test/test.component';
import { CtrlComponent } from './cmp/ctrl/ctrl.component';
import { MenuComponent } from './cmp/menu/menu.component';
import { ItemComponent } from './cmp/ctrl/item/item.component';
import { MatCardModule } from '@angular/material/card';
import { MatButtonModule } from '@angular/material/button';
import { MatInputModule } from '@angular/material/input';
import { MatFormFieldModule } from '@angular/material/form-field';
import { FormsModule } from '@angular/forms';
import { AttributeEditorComponent } from './cmp/ctrl/item/attribute-editor/attribute-editor.component';
import { AttributeComponent } from './cmp/ctrl/item/attribute-editor/attribute/attribute.component';
import { HttpClientModule } from '@angular/common/http';

@NgModule({
  declarations: [
    AppComponent,
    TestComponent,
    CtrlComponent,
    MenuComponent,
    ItemComponent,
    AttributeEditorComponent,
    AttributeComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    BrowserAnimationsModule,
    MatCardModule,
    MatButtonModule,
    MatFormFieldModule,
    MatInputModule,
    FormsModule,
    HttpClientModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
