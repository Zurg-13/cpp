import { ComponentFixture, TestBed } from '@angular/core/testing';

import { CtrlComponent } from './ctrl.component';

describe('CtrlComponent', () => {
  let component: CtrlComponent;
  let fixture: ComponentFixture<CtrlComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ CtrlComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(CtrlComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
