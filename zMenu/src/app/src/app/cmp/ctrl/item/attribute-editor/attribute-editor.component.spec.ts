import { ComponentFixture, TestBed } from '@angular/core/testing';

import { AttributeEditorComponent } from './attribute-editor.component';

describe('AttributeEditorComponent', () => {
  let component: AttributeEditorComponent;
  let fixture: ComponentFixture<AttributeEditorComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ AttributeEditorComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(AttributeEditorComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
