import {AnimationController} from "@ionic/angular";
import {animate, state, style, transition, trigger} from "@angular/animations";

const animationCtrl = new AnimationController();


export const app_animation = ((_: HTMLElement, opts: any) => {
  // create root transition
  const rootTransition = animationCtrl
    .create()
    .duration(opts.duration || 200)
    .easing('cubic-bezier(0.7,0,0.3,1)');

  const enterTransition = animationCtrl.create().addElement(opts.enteringEl);
  const exitTransition = animationCtrl.create().addElement(opts.leavingEl);

  enterTransition.fromTo('opacity', '0', '1');
  exitTransition.fromTo('opacity', '1', '0');

  let value: number = 2.5;

  if (opts.direction === 'forward') {
    enterTransition.fromTo('transform', `translateY(${value}%)`, 'translateY(0%)');
    exitTransition.fromTo('transform', 'translateX(0%)', `translateY(-${value}%)`);
  } else {
    enterTransition.fromTo('transform', `translateY(-${value}%)`, 'translateY(0%)');
    exitTransition.fromTo('transform', 'translateY(0%)', `translateY(${value}%)`);
  }

  rootTransition.addAnimation([enterTransition, exitTransition]);
  return rootTransition;
});


export const alert_animations: any[] = [
  //Programování animací skrze Angular
  trigger('AlertAnimation', [
    state('void', style({ opacity: 0, transform: 'translate(-50%, 200%)' })),
    transition(':enter', [
      animate(200, style({ opacity: 1, transform: 'translate(-50%, 0%)' }))
    ]),
    transition(':leave', [
      animate(200, style({ opacity: 1, transform: 'translate(-50%, 200%)' }))
    ]),
  ]),

  trigger('AlertAnimationTop', [
    state('void', style({ opacity: 0, width: "0%", transform: 'translate(-50%, -200%)' })),
    transition(':enter', [
      animate(200, style({ opacity: 1, transform: 'translate(-50%, 0%)' })),
      animate('200ms ease-in-out', style({ width: '80%' }))
    ]),
    transition(':leave', [
      animate(200, style({ opacity: 1, transform: 'translate(-50%, -200%)' }))
    ]),
  ]),

  trigger('AlertAnimationBackground', [
    state('void', style({ opacity: 0 })),
    transition(':enter', [
      animate(150, style({ opacity: 1 }))
    ]),
    transition(':leave', [
      animate(150, style({ opacity: 0 }))
    ]),
  ]),
]
